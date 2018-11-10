#include <src/adcs/magnetorquer_control.h>
#include <src/adcs/runnable_orientation_control.h>
#include <src/adcs/runnable_pre_deployment_magnetometer_poller.h>
#include <src/adcs/state_estimators/location_estimator.h>
#include <src/board/board.h>
#include <src/board/debug_interface/debug_stream.h>
#include <src/board/i2c/io_expander/io_expander.h>
#include <src/board/uart/uart.h>
#include <src/config/satellite.h>
#include <src/config/stacks.h>
#include <src/database/eeprom.h>
#include <src/database/flash_memory/flash_storables/reset_info_container.h>
#include <src/database/sd_card.h>
#include <src/init/post_bios_initialiser.h>
#include <src/init/test_initialiser.h>
#include <src/messages/Tle.pb.h>
#include <src/payload_processor/payload_processor.h>
#include <src/payload_processor/runnable_payload_processor.h>
#include <src/payload_processor/uplinks/tle_update_uplink.h>
#include <src/sensors/measurable_manager.h>
#include <src/sensors/runnable_system_health_check.h>
#include <src/tasks/task_holder.h>
#include <src/telecomms/antenna.h>
#include <src/telecomms/lithium.h>
#include <src/telecomms/lithium_commands/fast_pa_command.h>
#include <src/telecomms/runnable_beacon.h>
#include <src/telecomms/runnable_continuous_transmit_shutoff.h>
#include <src/telecomms/runnable_lithium_listener.h>
#include <src/util/flash.h>
#include <src/util/runnable_console_listener.h>
#include <src/util/runnable_console_logger.h>
#include <src/util/runnable_memory_logger.h>
#include <src/util/runnable_time_source.h>
#include <src/util/satellite_power.h>
#include <src/util/satellite_time_source.h>
#include <src/util/system_watchdog.h>
#include <src/util/task_utils.h>
#include <string.h>
#include <ti/sysbios/knl/Semaphore.h>
#include <xdc/runtime/Log.h>

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

void PostBiosInitialiser::InitRadioListener() {
    TaskHolder* radio_listener =
        new TaskHolder(kRadioListenerStackSize, "RadioListener", 12,
                       new RunnableLithiumListener());
    radio_listener->Start();
    Log_info0("Radio receiver started");
}

void PostBiosInitialiser::InitContinuousTransmitShutoff() {
    TaskHolder* transmit_shutoff =
        new TaskHolder(kTransmitShutoffStackSize, "Transmit Shutoff", 11,
                       new RunnableContinuousTransmitShutoff());
    transmit_shutoff->Start();
    Log_info0("Lithium failsafe started");
}

void PostBiosInitialiser::RunUnitTests() {
    TaskHolder* test_task = new TaskHolder(kUnitTestsStackSize, "Unit Tests", 3,
                                           TestInitialiser::GetInstance());
    test_task->Start();
}

void PostBiosInitialiser::InitBeacon() {
    FastPaCommand fast_pa_command(kNominalLithiumPowerLevel);
    if (!Lithium::GetInstance()->DoCommand(&fast_pa_command)) {
        Log_error0("Failed to initialise Lithium power amplifier setting");
    }

    TaskHolder* beacon_task =
        new TaskHolder(kBeaconStackSize, "Beacon", 8, new RunnableBeacon());
    beacon_task->Start();
    Log_info0("Beacon started");
}

void PostBiosInitialiser::InitPayloadProcessor() {
    TaskHolder* payload_processor_task =
        new TaskHolder(kPayloadProcessorStackSize, "PayloadProcessor", 6,
                       new RunnablePayloadProcessor());

    payload_processor_task->Start();
    Log_info0("Payload processor started");
}

void PostBiosInitialiser::InitOrientationControl() {
    if (kRunMagnetorquersAtConstantPower) {
        // Rather than start orientation control, just blast the
        // magnetorquers
        MagnetorquerControl::SetMagnetorquersPowerFraction(
            kMagnetorquerPowerFractionX, kMagnetorquerPowerFractionY,
            kMagnetorquerPowerFractionZ);
        Log_info0("Magnetorquers set to constant power");
    } else {
        Semaphore_post(RunnablePreDeploymentMagnetometerPoller::
                           kill_task_on_orientation_control_begin_semaphore);
        // Set up timer for orientation control loop
        RunnableOrientationControl::SetupControlLoopTimer();

        // Set up timer for degaussing routine
        MagnetorquerControl::SetupDegaussingPolaritySwitchTimer();

        // Set up semaphore for the beacon to safely turn off the
        // flight systems board
        RunnableOrientationControl::SetupBeaconOverSemaphore();

        // TODO(rskew) review priority
        TaskHolder* orientation_control_task =
            new TaskHolder(kOrientationControlStackSize, "OrientationControl",
                           7, new RunnableOrientationControl());
        Mailbox_Params_init(
            &LocationEstimator::tle_update_uplink_mailbox_params);
        Mailbox_Handle tle_update_uplink_mailbox_handle = Mailbox_create(
            sizeof(Tle), 1,
            &LocationEstimator::tle_update_uplink_mailbox_params, NULL);
        if (tle_update_uplink_mailbox_handle == NULL) {
            etl::exception e("Unable to create TLE update command mailbox",
                             __FILE__, __LINE__);
            throw e;
        }
        LocationEstimator::SetTleUpdateUplinkMailboxHandle(
            tle_update_uplink_mailbox_handle);
        TleUpdateUplink::SetTleUpdateUplinkMailboxHandle(
            tle_update_uplink_mailbox_handle);

        orientation_control_task->Start();
        Log_info0("Orientation control started");
    }
}

void PostBiosInitialiser::InitPreDeploymentMagnetometerPoller() {
    if (kRunMagnetorquersAtConstantPower == false) {
        RunnablePreDeploymentMagnetometerPoller::
            SetupKillTaskOnOrientationControlBeginSemaphore();
        // TODO(rskew) review priority
        TaskHolder* pre_deployment_magnetometer_poller_task =
            new TaskHolder(kPreDeploymentMagnetometerPollerStackSize,
                           "PreDeploymentMagnetometerPoller", 4,
                           new RunnablePreDeploymentMagnetometerPoller());
        pre_deployment_magnetometer_poller_task->Start();
    }
}

void PostBiosInitialiser::InitSystemHealthCheck() {
    TaskHolder* system_health_check_task =
        new TaskHolder(kSystemHealthCheckStackSize, "SystemHealthCheck", 5,
                       new RunnableSystemHealthCheck());
    system_health_check_task->Start();
    Log_info0("System healthcheck started");
}

void PostBiosInitialiser::InitHardware() {
    // TODO(akremor): Possible failure mode needs to be handled

    try {
        I2c::InitBusses();
    } catch (etl::exception& e) {
        MspException::LogException(e);
    }

    I2c* bus_a = new I2c(I2C_BUS_A);
    I2c* bus_b = new I2c(I2C_BUS_B);
    I2c* bus_c = new I2c(I2C_BUS_C);
    I2c* bus_d = new I2c(I2C_BUS_D);

    try {
        IoExpander::Init(bus_d);
    } catch (etl::exception& e) {
        MspException::LogException(e);
    }

    try {
        Flash::FlashLed();
        SatellitePower::Initialize();
        TaskUtils::SleepMilli(3000);
        TaskUtils::SleepMilli(1000);
        Flash::FlashLed();
        Flash::FlashLed();
        SatellitePower::RestorePowerToFlightSystems();
        TaskUtils::SleepMilli(1000);
        Flash::FlashLed();
        Flash::FlashLed();
        Flash::FlashLed();
        SatellitePower::RestorePowerToTelecoms();
    } catch (etl::exception& e) {
        MspException::LogException(e);
    }

    try {
        Eeprom::Init();
    } catch (etl::exception& e) {
        MspException::LogException(e);
    }

    try {
        MagnetorquerControl::Initialize();
    } catch (etl::exception& e) {
        MspException::LogException(e);
    }

    try {
        SdCard* sd = SdCard::GetInstance();
        sd->SdOpen();
        if (kFormatSdOnStartup) {
            sd->Format();
        }
    } catch (etl::exception& e) {
        MspException::LogException(e);
    }

    try {
        Antenna::GetAntenna()->InitAntenna(bus_d);
    } catch (etl::exception& e) {
        MspException::LogException(e);
    }

    try {
        Lithium::GetInstance();
    } catch (etl::exception& e) {
        MspException::LogException(e);
    }

    try {
        DebugStream::GetInstance();
    } catch (etl::exception& e) {
        MspException::LogException(e);
    }

    try {
        MeasurableManager::GetInstance()->Init(bus_a, bus_b, bus_c, bus_d);
    } catch (etl::exception& e) {
        // TODO(akremor): Possible failure mode needs to be handled
        // Pass exceptions up so that an incompletely initialised
        // measurable manager isn't used.
        // If a hardware sensor fails to be initialised, it should be
        // caught by the driver. Only exceptions from measurables, which should
        // be software problems, should get to here.
        MspException::LogException(e);
    }
}

void PostBiosInitialiser::InitMemoryLogger() {
    TaskHolder* memory_logger_task = new TaskHolder(
        kMemoryLoggerStackSize, "MemoryLogger", 11, new RunnableMemoryLogger());
    memory_logger_task->Start();
    Log_info0("Memory logger started");
}

void PostBiosInitialiser::InitTimeSource() {
    TaskHolder* time_source_task = new TaskHolder(
        kTimeSourceStackSize, "TimeSource", 10, new RunnableTimeSource());
    time_source_task->Start();
}

void PostBiosInitialiser::InitConsoleUart() {
    Uart* debug_uart = new Uart(UMBILICAL_CONSOLE);
    debug_uart->SetBaudRate(Uart::kBaud115200)
        ->SetReadTimeout(TaskUtils::MilliToCycles(kDebugUartReadTimeout))
        ->SetWriteTimeout(TaskUtils::MilliToCycles(kDebugUartWriteTimeout))
        ->Open();

    TaskHolder* console_uart_listener_task =
        new TaskHolder(kConsoleListenerStackSize, "UartListener", 12,
                       new RunnableConsoleListener(debug_uart));
    console_uart_listener_task->Start();
    Log_info0("Umbilical UART listener started");

    TaskHolder* console_uart_logger_task =
        new TaskHolder(kConsoleLoggerStackSize, "UartLogger", 7,
                       new RunnableConsoleLogger(debug_uart));
    console_uart_logger_task->Start();
    Log_info0("Umbilical UART logger started");
}

void PostBiosInitialiser::DeployAntenna() {
    // TODO(akremor): We should add a force-enable based on number of
    // reboots feature In case the satellite gets stuck in a boot loop or
    // similar, we don't want the timers to be operating each time
    SatelliteTimeSource::RealTimeWait(kAntennaDelaySeconds);
    Log_info0("Antenna deploying, can take awhile");
    Antenna::GetAntenna()->DeployAntenna();
}

void PostBiosInitialiser::PostBiosInit() {
    Log_info0("System has started");

    if (kDitlMode) {
        InitMemoryLogger();
    }

    try {
        // TODO(dingbenjamin): Init var length array pool
        MspException::Init();
        InitConsoleUart();
        InitHardware();
        InitTimeSource();  // Relies on I2C so needs to be post InitHardware()
        InitRadioListener();
        InitPayloadProcessor();
        InitContinuousTransmitShutoff();

#if defined TEST_CONFIGURATION
        RunUnitTests();
#endif

#if defined ORBIT_CONFIGURATION
        SystemWatchdog((uint32_t)SYS_WATCHDOG0);
        InitPreDeploymentMagnetometerPoller();
        DeployAntenna();
        InitOrientationControl();
        InitBeacon();
        InitSystemHealthCheck();
        Log_info0("System start up complete");
#endif
    } catch (etl::exception& e) {
        MspException::LogException(e);
        Log_error0("System start up failed");
        System_flush();
    }
}
