#include <src/adcs/magnetorquer_control.h>
#include <src/adcs/runnable_orientation_control.h>
#include <src/adcs/state_estimators/location_estimator.h>
#include <src/board/board.h>
#include <src/board/debug_interface/debug_stream.h>
#include <src/board/i2c/io_expander/io_expander.h>
#include <src/board/uart/uart.h>
#include <src/config/orientation_control_tuning_parameters.h>
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
#include <src/telecomms/runnable_antenna_burner.h>
#include <src/telecomms/runnable_beacon.h>
#include <src/telecomms/runnable_continuous_transmit_shutoff.h>
#include <src/telecomms/runnable_lithium_listener.h>
#include <src/util/runnable_console_listener.h>
#include <src/util/runnable_console_logger.h>
#include <src/util/runnable_memory_logger.h>
#include <src/util/runnable_power_manager.h>
#include <src/util/runnable_time_source.h>
#include <src/util/satellite_power.h>
#include <src/util/satellite_time_source.h>
#include <src/util/system_watchdog.h>
#include <src/util/tirtos_utils.h>
#include <string.h>
#include <ti/sysbios/gates/GateMutexPri.h>
#include <ti/sysbios/knl/Semaphore.h>
#include <xdc/runtime/Log.h>

PostBiosInitialiser::PostBiosInitialiser() {}

fnptr PostBiosInitialiser::GetRunnablePointer() {
    return &PostBiosInitialiser::PostBiosInit;
}

void PostBiosInitialiser::InitRadioListener(uint16_t stack_size) {
    TaskHolder* radio_listener = new TaskHolder(stack_size, "RadioListener", 12,
                                                new RunnableLithiumListener());
    radio_listener->Start();
    Log_info0("Radio receiver started");
}

void PostBiosInitialiser::InitContinuousTransmitShutoff(uint16_t stack_size) {
    TaskHolder* transmit_shutoff =
        new TaskHolder(stack_size, "Transmit Shutoff", 11,
                       new RunnableContinuousTransmitShutoff());
    transmit_shutoff->Start();
    Log_info0("Lithium failsafe started");
}

void PostBiosInitialiser::RunUnitTests(uint16_t stack_size) {
    TaskHolder* test_task = new TaskHolder(stack_size, "Unit Tests", 3,
                                           TestInitialiser::GetInstance());
    test_task->Start();
}

void PostBiosInitialiser::InitBeacon(uint16_t stack_size, bool limp_mode) {
    if (!Lithium::GetInstance()->DoFastPa(kNominalLithiumPowerLevel)) {
        Log_error0("Failed to initialise Lithium power amplifier setting");
    }

    TaskHolder* beacon_task =
        new TaskHolder(stack_size, "Beacon", 8, new RunnableBeacon(limp_mode));
    beacon_task->Start();
    Log_info0("Beacon started");
}

void PostBiosInitialiser::InitPayloadProcessor(uint16_t stack_size) {
    TaskHolder* payload_processor_task = new TaskHolder(
        stack_size, "PayloadProcessor", 6, new RunnablePayloadProcessor());

    payload_processor_task->Start();
    Log_info0("Payload processor started");
}

void PostBiosInitialiser::InitOrientationControl(uint16_t stack_size) {
    if (kRunMagnetorquersAtConstantPower) {
        // Rather than start orientation control, just blast the
        // magnetorquers
        MagnetorquerControl::SetMagnetorquersPowerFraction(
            kMagnetorquerPowerFractionX, kMagnetorquerPowerFractionY,
            kMagnetorquerPowerFractionZ);
        Log_info0("Magnetorquers set to constant power");
    } else {
        // Set up timer for orientation control loop
        RunnableOrientationControl::SetupControlLoopTimer();

        // Set up timer for degaussing routine
        MagnetorquerControl::SetupDegaussingPolaritySwitchTimer();

        // TODO(rskew) review priority
        TaskHolder* orientation_control_task =
            new TaskHolder(stack_size, "OrientationControl", 7,
                           new RunnableOrientationControl());
        Mailbox_Params_init(
            &LocationEstimator::tle_update_uplink_mailbox_params);
        Mailbox_Handle tle_update_uplink_mailbox_handle = Mailbox_create(
            sizeof(Tle), 1,
            &LocationEstimator::tle_update_uplink_mailbox_params, NULL);
        if (tle_update_uplink_mailbox_handle == NULL) {
            throw MspException("Unable to create TLE update command mailbox",
                               kTleUpdateMailboxFail, __FILE__, __LINE__);
        }
        LocationEstimator::SetTleUpdateUplinkMailboxHandle(
            tle_update_uplink_mailbox_handle);
        TleUpdateUplink::SetTleUpdateUplinkMailboxHandle(
            tle_update_uplink_mailbox_handle);

        orientation_control_task->Start();
        Log_info0("Orientation control started");
    }
}

void PostBiosInitialiser::InitSystemHealthCheck(uint16_t stack_size) {
    TaskHolder* system_health_check_task = new TaskHolder(
        stack_size, "SystemHealthCheck", 5, new RunnableSystemHealthCheck());
    system_health_check_task->Start();
    Log_info0("System healthcheck started");
}

void PostBiosInitialiser::InitHardware() {
    // TODO(akremor): Possible failure mode needs to be handled

    try {
        I2c::InitBusses();
    } catch (MspException& e) {
        MspException::LogException(e, kI2cInitCatch);
    }

    I2c* bus_a = new I2c(I2C_BUS_A);
    I2c* bus_b = new I2c(I2C_BUS_B);
    I2c* bus_c = new I2c(I2C_BUS_C);
    I2c* bus_d = new I2c(I2C_BUS_D);

    try {
        IoExpander::Init(bus_d);
    } catch (MspException& e) {
        MspException::LogException(e, kIoExpanderInitCatch);
    }

    try {
        Eeprom::Init();
    } catch (MspException& e) {
        MspException::LogException(e, kEepromInitCatch);
    }

    try {
        MagnetorquerControl::Initialize();
    } catch (MspException& e) {
        MspException::LogException(e, kMagnetorquerControlInitCatch);
    }

    try {
        if (kSdCardAvailable) {
            SdCard* sd = SdCard::GetInstance();
            sd->SdOpen();
            if (kFormatSdOnStartup) {
                sd->Format();
            }
        }
    } catch (MspException& e) {
        MspException::LogException(e, kSdInitCatch);
    }

    try {
        Antenna::GetAntenna()->InitAntenna(bus_d);
    } catch (MspException& e) {
        MspException::LogException(e, kAntennaInitCatch);
    }

    try {
        DebugStream::GetInstance();
    } catch (MspException& e) {
        MspException::LogException(e, kDebugStreamInitCatch);
    }

    try {
        Bms* bms_bus_d =
            new Bms(bus_d, 0x68, NULL, I2cMultiplexer::kMuxNoChannel);
        Bms* bms_bus_c =
            new Bms(bus_c, 0x68, NULL, I2cMultiplexer::kMuxNoChannel);
        SatellitePower::Initialize(bms_bus_d, bms_bus_c);
    } catch (MspException& e) {
        MspException::LogException(e, kSatellitePowerInitCatch);
    }

    try {
        MeasurableManager::GetInstance()->Init(bus_a, bus_b, bus_c, bus_d);
    } catch (MspException& e) {
        // TODO(akremor): Possible failure mode needs to be handled
        // Pass exceptions up so that an incompletely initialised
        // measurable manager isn't used.
        // If a hardware sensor fails to be initialised, it should be
        // caught by the driver. Only exceptions from measurables, which should
        // be software problems, should get to here.
        MspException::LogException(e, kMeasurableManagerInitCatch);
    }

    InitTimeSource();

    try {
        SatellitePower::RestorePowerToTelecoms();
        TirtosUtils::SleepMilli(1000);
        SatellitePower::RestorePowerToFlightSystems();
    } catch (MspException& e) {
        MspException::LogException(e, kSatellitePowerPoweronCatch);
    }

    try {
        Lithium::GetInstance();
    } catch (MspException& e) {
        MspException::LogException(e, kLithiumInitCatch);
    }
}

void PostBiosInitialiser::InitMemoryLogger(uint16_t stack_size) {
    if (kDitlMode) {
        TaskHolder* memory_logger_task = new TaskHolder(
            stack_size, "MemoryLogger", 11, new RunnableMemoryLogger());
        memory_logger_task->Start();
        Log_info0("Memory logger started");
    }
}

void PostBiosInitialiser::InitTimeSource(uint16_t stack_size) {
    TaskHolder* time_source_task =
        new TaskHolder(stack_size, "TimeSource", 10, new RunnableTimeSource());
    time_source_task->Start();
}

void PostBiosInitialiser::InitConsoleUart(uint16_t logger_stack_size,
                                          uint16_t listener_stack_size) {
    Uart* debug_uart = new Uart(UMBILICAL_CONSOLE);
    debug_uart->SetBaudRate(Uart::kBaud115200)
        ->SetReadTimeout(TirtosUtils::MilliToCycles(kDebugUartReadTimeout))
        ->SetWriteTimeout(TirtosUtils::MilliToCycles(kDebugUartWriteTimeout))
        ->Open();

    TaskHolder* console_uart_logger_task =
        new TaskHolder(logger_stack_size, "UartLogger", 7,
                       new RunnableConsoleLogger(debug_uart));
    console_uart_logger_task->Start();
    Log_info0("Umbilical UART logger started");

    TaskHolder* console_uart_listener_task =
        new TaskHolder(listener_stack_size, "UartListener", 12,
                       new RunnableConsoleListener(debug_uart));
    console_uart_listener_task->Start();
    Log_info0("Umbilical UART listener started");
}

void PostBiosInitialiser::InitAntennaBurner(uint16_t stack_size) {
    // TODO (rskew) review priority
    TaskHolder* antenna_burner_task = new TaskHolder(
        stack_size, "AntennaBurner", 6, new RunnableAntennaBurner());
    antenna_burner_task->Start();
}

void PostBiosInitialiser::EjectionWait() {
    if (!kInstantDeploymentWaits) {
        try {
            Time current_time = SatelliteTimeSource::GetTime();
            if (current_time.is_valid) {
                uint32_t time_since_ejection_ms =
                    current_time.timestamp_ms - kTimeSourceDeployMs;
                if (time_since_ejection_ms < kEjectionWaitMs) {
                    Log_info0("Post-deployment wait starting");
                    TirtosUtils::SleepMilli(kEjectionWaitMs -
                                            time_since_ejection_ms);
                    Log_info0("Post-deployment wait finished");
                }
            } else {
                Log_warning0("Invalid reading from RTC");
                Log_info0("Post-deployment wait starting");
                TirtosUtils::SleepMilli(kEjectionWaitMs);
                Log_info0("Post-deployment wait finished");
            }
        } catch (MspException& e) {
            MspException::LogException(e, kEjectionWaitCatch);
            Log_error0("Unable to retrieve time from RTC");
            Log_info0("Post-deployment wait starting");
            TirtosUtils::SleepMilli(kEjectionWaitMs);
            Log_info0("Post-deployment wait finished");
        }
    } else {
        Log_info0("Skipping post-deployment wait");
    }
}

void PostBiosInitialiser::BeaconWait() {
    TirtosUtils::SleepMilli(kBeaconWaitMs);
}

void PostBiosInitialiser::InitPowerManager(uint16_t stack_size) {
    // TODO (rskew) review priority
    TaskHolder* power_manager_task = new TaskHolder(
        stack_size, "PowerManager", 8, new RunnablePowerManager());
    power_manager_task->Start();
}

void PostBiosInitialiser::PostBiosInit() {
    Log_info0("System has started");

    if (kDitlMode) {
        InitMemoryLogger();
    }

    try {
        // TODO(dingbenjamin): Init var length array pool
        InitConsoleUart();
        InitMemoryLogger();
        MspException::Init();
        InitHardware();
        InitRadioListener();
        InitPayloadProcessor();
        InitContinuousTransmitShutoff();

#if defined TEST_CONFIGURATION
        RunUnitTests();
#endif

#if defined ORBIT_CONFIGURATION
        SystemWatchdog((uint32_t)SYS_WATCHDOG0);
        InitSystemHealthCheck();
        InitPowerManager();
        BeaconWait();
        InitBeacon();
        EjectionWait();
        InitOrientationControl();
        InitAntennaBurner();
        Log_info0("System start up complete");
#endif
    } catch (MspException& e) {
        MspException::LogException(e, kPostBiosInitCatch);
        Log_error0("System start up failed");
        System_flush();
    }
}
