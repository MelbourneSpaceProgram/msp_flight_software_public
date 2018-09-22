#include <src/adcs/magnetorquer_control.h>
#include <src/adcs/runnable_orientation_control.h>
#include <src/adcs/runnable_pre_deployment_magnetometer_poller.h>
#include <src/adcs/state_estimators/location_estimator.h>
#include <src/board/board.h>
#include <src/board/debug_interface/debug_stream.h>
#include <src/board/uart/uart.h>
#include <src/config/satellite.h>
#include <src/config/stacks.h>
#include <src/data_dashboard/runnable_data_dashboard.h>
#include <src/database/eeprom.h>
#include <src/database/sd_card.h>
#include <src/init/post_bios_initialiser.h>
#include <src/init/test_initialiser.h>
#include <src/messages/Tle.pb.h>
#include <src/payload_processor/commands/tle_update_command.h>
#include <src/payload_processor/payload_processor.h>
#include <src/payload_processor/runnable_console_uart_listener.h>
#include <src/payload_processor/runnable_payload_processor.h>
#include <src/sensors/i2c_measurable_manager.h>
#include <src/system/runnable_state_management.h>
#include <src/system/runnable_system_health_check.h>
#include <src/system/state_manager.h>
#include <src/tasks/task_holder.h>
#include <src/telecomms/antenna.h>
#include <src/telecomms/lithium.h>
#include <src/telecomms/lithium_commands/fast_pa_command.h>
#include <src/telecomms/runnable_beacon.h>
#include <src/telecomms/runnable_lithium_listener.h>
#include <src/util/runnable_memory_logger.h>
#include <src/util/runnable_time_source.h>
#include <src/util/satellite_time_source.h>
#include <src/util/system_watchdog.h>
#include <src/util/task_utils.h>
#include <ti/sysbios/knl/Semaphore.h>
#include <xdc/runtime/Log.h>
#include <src/telecomms/lithium_commands/fast_pa_command.h>

PostBiosInitialiser::PostBiosInitialiser() {}

fnptr PostBiosInitialiser::GetRunnablePointer() {
    return &PostBiosInitialiser::PostBiosInit;
}

Uart* PostBiosInitialiser::InitDebugUart() {
    Uart* debug_uart = new Uart(UMBILICAL_CONSOLE);
    debug_uart->SetBaudRate(Uart::kBaud115200)
        ->SetReadTimeout(TaskUtils::MilliToCycles(kDebugUartReadTimeout))
        ->SetWriteTimeout(TaskUtils::MilliToCycles(kDebugUartWriteTimeout))
        ->Open();
    return debug_uart;
}

void PostBiosInitialiser::InitSingletons(I2c* bus_a, I2c* bus_b, I2c* bus_c,
                                         I2c* bus_d) {
    try {
        DebugStream::GetInstance();
    } catch (etl::exception& e) {
        // TODO(akremor): Possible failure mode needs to be handled
    }

    try {
        Antenna::GetAntenna()->InitAntenna(bus_d);
    } catch (etl::exception& e) {
        // TODO(akremor): Possible failure mode needs to be handled
    }

    try {
        Lithium::GetInstance();
    } catch (etl::exception& e) {
        // TODO(akremor): Possible failure mode needs to be handled
    }

    try {
        StateManager::GetStateManager()->CreateStateMachines();
    } catch (etl::exception& e) {
        // TODO(akremor): Possible failure mode needs to be handled
    }

    try {
        I2cMeasurableManager::GetInstance()->Init(bus_a, bus_b, bus_c, bus_d);
    } catch (etl::exception& e) {
        // TODO(akremor): Possible failure mode needs to be handled
        // Pass exceptions up so that an incompletely initialised
        // measurable manager isn't used.
        // If a hardware sensor fails to be initialised, it should be
        // caught by the driver. Only exceptions from measurables, which should
        // be software problems, should get to here.
        throw e;
    }
}

void PostBiosInitialiser::InitRadioListener() {
    TaskHolder* radio_listener =
        new TaskHolder(radio_listener_stack_size, "RadioListener", 12,
                       new RunnableLithiumListener());
    radio_listener->Start();
}

void PostBiosInitialiser::RunUnitTests() {
    TaskHolder* test_task = new TaskHolder(unit_tests_stack_size, "Unit Tests",
                                           3, TestInitialiser::GetInstance());
    test_task->Start();
}

void PostBiosInitialiser::InitStateManagement() {
    StateManager::GetStateManager();

    TaskHolder* state_management_task = new TaskHolder(
        1024, "StateManagement", 9, new RunnableStateManagement());
    state_management_task->Start();
}

void PostBiosInitialiser::InitBeacon() {
    TaskHolder* beacon_task =
        new TaskHolder(beacon_stack_size, "Beacon", 8, new RunnableBeacon());
    beacon_task->Start();
}

void PostBiosInitialiser::InitPayloadProcessor() {
    TaskHolder* payload_processor_task = new TaskHolder(
        2048, "PayloadProcessor", 6, new RunnablePayloadProcessor());

    payload_processor_task->Start();
}

void PostBiosInitialiser::InitDataDashboard() {
    // TODO(rskew) review priority
    TaskHolder* data_dashboard_task =
        new TaskHolder(4096, "DataDashboard", 2, new RunnableDataDashboard());
    data_dashboard_task->Start();
}

void PostBiosInitialiser::InitOrientationControl() {
    // Set up timer for orientation control loop
    RunnableOrientationControl::SetupControlLoopTimer();

    // Set up timer for degaussing routine
    MagnetorquerControl::SetupDegaussingPolaritySwitchTimer();

    // TODO(rskew) review priority
    TaskHolder* orientation_control_task = new TaskHolder(
        4096, "OrientationControl", 7, new RunnableOrientationControl());
    Mailbox_Params_init(&LocationEstimator::tle_update_command_mailbox_params);
    Mailbox_Handle tle_update_command_mailbox_handle = Mailbox_create(
        sizeof(Tle), 1, &LocationEstimator::tle_update_command_mailbox_params,
        NULL);
    if (tle_update_command_mailbox_handle == NULL) {
        etl::exception e("Unable to create TLE update command mailbox",
                         __FILE__, __LINE__);
        throw e;
    }
    LocationEstimator::SetTleUpdateCommandMailboxHandle(
        tle_update_command_mailbox_handle);
    TleUpdateCommand::SetTleUpdateCommandMailboxHandle(
        tle_update_command_mailbox_handle);

    orientation_control_task->Start();
}

void PostBiosInitialiser::InitPreDeploymentMagnetometerPoller() {
    RunnablePreDeploymentMagnetometerPoller::
        SetupKillTaskOnOrientationControlBeginSemaphore();
    // TODO(rskew) review priority
    TaskHolder* pre_deployment_magnetometer_poller_task = new TaskHolder(
        // works with this little stack?
        1024, "PreDeploymentMagnetometerPoller", 4,
        new RunnablePreDeploymentMagnetometerPoller());
    pre_deployment_magnetometer_poller_task->Start();
}

void PostBiosInitialiser::InitSystemHealthCheck(Uart* debug_uart) {
    TaskHolder* system_health_check_task =
        new TaskHolder(4096, "SystemHealthCheck", 5,
                       new RunnableSystemHealthCheck(debug_uart));
    system_health_check_task->Start();
}

void PostBiosInitialiser::InitHardware() {
    try {
        I2c::InitBusses();
    } catch (etl::exception& e) {
        EtlUtils::LogException(e);
        // TODO(akremor): Possible failure mode needs to be handled
    }

    try {
        Eeprom::Init();
    } catch (etl::exception& e) {
        EtlUtils::LogException(e);
        // TODO(akremor): Possible failure mode needs to be handled
    }

    try {
        MagnetorquerControl::Initialize();
    } catch (etl::exception& e) {
        EtlUtils::LogException(e);
        // TODO(akremor): Possible failure mode needs to be handled
    }

    try {
        SdCard::SdOpen();
        if (kFormatSdOnStartup) {
            SdCard::Format();
        }
    } catch (etl::exception& e) {
        EtlUtils::LogException(e);
        // TODO(akremor): Possible failure mode needs to be handled
    }
}

void PostBiosInitialiser::InitMemoryLogger() {
    TaskHolder* memory_logger_task =
        new TaskHolder(memory_logger_stack_size, "MemoryLogger", 11,
                       new RunnableMemoryLogger());
    memory_logger_task->Start();
}

void PostBiosInitialiser::InitTimeSource() {
    TaskHolder* time_source_task = new TaskHolder(
        time_source_stack_size, "TimeSource", 10, new RunnableTimeSource());
    time_source_task->Start();
}

void PostBiosInitialiser::InitConsoleUartListener(Uart* debug_uart) {
    TaskHolder* console_uart_listener_task = new TaskHolder(
        1536, "UartListener", 12, new RunnableConsoleUartListener(debug_uart));
    console_uart_listener_task->Start();
}

void PostBiosInitialiser::PostBiosInit() {
    Log_info0("System has started");

    RunnableSystemHealthCheck::Init();

    if (kDitlMode) {
        InitMemoryLogger();
    }

    try {
        // TODO(dingbenjamin): Init var length array pool

        InitHardware();
        InitTimeSource();  // Relies on I2C so needs to be post InitHardware()

        I2c* bus_a = new I2c(I2C_BUS_A);
        I2c* bus_b = new I2c(I2C_BUS_B);
        I2c* bus_c = new I2c(I2C_BUS_C);
        I2c* bus_d = new I2c(I2C_BUS_D);

        InitSingletons(bus_a, bus_b, bus_c, bus_d);

        InitRadioListener();
        Log_info0("Radio receiver started");

        FastPaCommand fast_pa_command(kNominalLithiumPowerLevel);
        if (!Lithium::GetInstance()->DoCommand(&fast_pa_command)) {
            Log_error0("Failed to initialise Lithium power amplifier setting");
        }

        InitPayloadProcessor();
        Log_info0("Payload processor started");

        Uart* debug_uart = InitDebugUart();
        InitConsoleUartListener(debug_uart);
        Log_info0("UART listener started");

        // The satellite now runs either the unit tests or the in-orbit
        // configuration for memory reasons.
        // To swap between the two configurations:
        // - Right click on the project (MSP)
        // - Build Configurations
        // - Set Active
        // - Choose Orbit or TIRTOS Build

#if defined TEST_CONFIGURATION
        RunUnitTests();
#endif

#if defined ORBIT_CONFIGURATION
        SystemWatchdog((uint32_t)SYS_WATCHDOG0);
        InitStateManagement();
        if (kHilAvailable) {
            InitDataDashboard();
        }

        if (kRunMagnetorquersAtConstantPower == false) {
            InitPreDeploymentMagnetometerPoller();
        }

        // TODO(akremor): We should add a force-enable based on number of
        // reboots feature In case the satellite gets stuck in a boot loop or
        // similar, we don't want the timers to be operating each time
        Log_info0("Starting beacon delay timer");
        SatelliteTimeSource::RealTimeWait(kBeaconDelaySeconds);
        Log_info0("Beacon delay timer finished");

        SatelliteTimeSource::RealTimeWait(kAntennaDelaySeconds);
        Log_info0("Antenna deploying, can take awhile");
        Antenna::GetAntenna()->DeployAntenna();
        if (kRunMagnetorquersAtConstantPower == false) {
            Semaphore_post(
                RunnablePreDeploymentMagnetometerPoller::
                    kill_task_on_orientation_control_begin_semaphore);
        }
        InitBeacon();
        Log_info0("Beacon started");

        if (kRunMagnetorquersAtConstantPower) {
            // Rather than start orientation control, just blast the
            // magnetorquers
            MagnetorquerControl::SetMagnetorquersPowerFraction(
                kMagnetorquerPowerFractionX, kMagnetorquerPowerFractionY,
                kMagnetorquerPowerFractionZ);
            Log_info0("Magnetorquers set to constant power");
        } else {
            InitOrientationControl();
            Log_info0("Orientation control started");
        }

        InitSystemHealthCheck(debug_uart);
        Log_info0("System healthcheck started");

        Log_info0("System start up complete");
#endif
    } catch (etl::exception& e) {
        EtlUtils::LogException(e);
        System_flush();
    }
}
