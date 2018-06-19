#include <src/adcs/magnetorquer_control.h>
#include <src/adcs/runnable_orientation_control.h>
#include <src/adcs/runnable_pre_deployment_magnetometer_poller.h>
#include <src/adcs/state_estimators/location_estimator.h>
#include <src/board/debug_interface/debug_stream.h>
#include <src/data_dashboard/runnable_data_dashboard.h>
#include <src/database/eeprom.h>
#include <src/database/sd_card.h>
#include <src/init/post_bios_initialiser.h>
#include <src/init/test_initialiser.h>
#include <src/messages/Tle.pb.h>
#include <src/payload_processor/commands/tle_update_command.h>
#include <src/payload_processor/payload_processor.h>
#include <src/payload_processor/runnable_payload_processor.h>
#include <src/sensors/i2c_measurable_manager.h>
#include <src/system/state_manager.h>
#include <src/system/tasks/runnable_state_management.h>
#include <src/system/tasks/runnable_system_health_check.h>
#include <src/tasks/task_holder.h>
#include <src/telecomms/antenna.h>
#include <src/telecomms/lithium.h>
#include <src/telecomms/runnable_beacon.h>
#include <src/telecomms/runnable_lithium_listener.h>
#include <src/util/runnable_memory_logger.h>
#include <src/util/runnable_time_source.h>
#include <src/util/satellite_time_source.h>
#include <src/util/system_watchdog.h>
#include <src/util/task_utils.h>
#include <ti/sysbios/knl/Semaphore.h>
#include <xdc/runtime/Log.h>

PostBiosInitialiser::PostBiosInitialiser() {}

fnptr PostBiosInitialiser::GetRunnablePointer() {
    return &PostBiosInitialiser::PostBiosInit;
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
    }
}

void PostBiosInitialiser::InitRadioListener() {
    TaskHolder* radio_listener = new TaskHolder(1200, "RadioListener", 11,
                                                new RunnableLithiumListener());
    radio_listener->Init();
}

void PostBiosInitialiser::RunUnitTests() {
    TaskHolder* test_task =
        new TaskHolder(11000, "Unit Tests", 7, TestInitialiser::GetInstance());
    test_task->Init();
}

void PostBiosInitialiser::InitStateManagement() {
    // TODO(rskew) review priority
    StateManager* state_manager = StateManager::GetStateManager();

    TaskHolder* state_management_task = new TaskHolder(
        1024, "StateManagement", 11, new RunnableStateManagement());
    state_management_task->Init();
}

void PostBiosInitialiser::InitBeacon() {
    TaskHolder* beacon_task =
        new TaskHolder(1536, "Beacon", 12, new RunnableBeacon());
    beacon_task->Init();
}

void PostBiosInitialiser::InitPayloadProcessor() {
    TaskHolder* payload_processor_task = new TaskHolder(
        1536, "PayloadProcessor", 12, new RunnablePayloadProcessor());
    payload_processor_task->Init();
}

void PostBiosInitialiser::InitDataDashboard() {
    // TODO(rskew) review priority
    TaskHolder* data_dashboard_task =
        new TaskHolder(4096, "DataDashboard", 5, new RunnableDataDashboard());
    data_dashboard_task->Init();
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

    orientation_control_task->Init();
}

TaskHolder* PostBiosInitialiser::InitPreDeploymentMagnetometerPoller() {
    RunnablePreDeploymentMagnetometerPoller::
        SetupKillTaskOnOrientationControlBeginSemaphore();
    // TODO(rskew) review priority
    TaskHolder* pre_deployment_magnetometer_poller_task = new TaskHolder(
        // works with this little stack?
        1024, "PreDeploymentMagnetometerPoller", 5,
        new RunnablePreDeploymentMagnetometerPoller());
    pre_deployment_magnetometer_poller_task->Init();
    return pre_deployment_magnetometer_poller_task;
}
void PostBiosInitialiser::InitSystemHealthCheck() {
    TaskHolder* system_health_check_task = new TaskHolder(
        1536, "SystemHealthCheck", 12, new RunnableSystemHealthCheck());
    system_health_check_task->Init();
}

void PostBiosInitialiser::DeployAntenna() {
    Antenna* antenna = Antenna::GetAntenna();
    if (!antenna->IsDoorsOpen()) {
        Log_info0("Trying safe deploy");
        antenna->SafeDeploy();
    }
    if (!antenna->IsDoorsOpen()) {
        Log_info0("Trying force deploy");
        antenna->ForceDeploy();
    }
    if (antenna->IsDoorsOpen()) {
        Log_info0("Antenna deployed");
    } else {
        Log_error0("Antenna failed to deploy");
    }
}

void PostBiosInitialiser::InitHardware() {
    try {
        I2c::InitBusses();
    } catch (etl::exception e) {
        // TODO(akremor): Possible failure mode needs to be handled
    }

    try {
        Eeprom::Init();
    } catch (etl::exception e) {
        // TODO(akremor): Possible failure mode needs to be handled
    }

    try {
        MagnetorquerControl::Initialize();
    } catch (etl::exception e) {
        // TODO(akremor): Possible failure mode needs to be handled
    }

    try {
        SdCard::SdOpen();
    } catch (etl::exception e) {
        // TODO(akremor): Possible failure mode needs to be handled
    }
}

void PostBiosInitialiser::InitMemoryLogger() {
    TaskHolder* memory_logger_task =
        new TaskHolder(1024, "MemoryLogger", 13, new RunnableMemoryLogger());
    memory_logger_task->Init();
}

void PostBiosInitialiser::InitTimeSource() {
    // Observed stack peak was 1200, so +300 for future changes and any
    // pre-emption that may occur
    TaskHolder* time_source_task =
        new TaskHolder(1500, "TimeSource", 13, new RunnableTimeSource());
    time_source_task->Init();
}

void PostBiosInitialiser::PostBiosInit() {
    Log_info0("System has started");
    SystemWatchdog(0);
    InitMemoryLogger();

    try {
        // TODO(dingbenjamin): Init var length array pool

        InitHardware();
        InitTimeSource();  // Relies on I2C so needs to be post InitHardware()

        I2c* bus_a = new I2c(I2C_BUS_A);
        I2c* bus_b = new I2c(I2C_BUS_B);
        I2c* bus_c = new I2c(I2C_BUS_C);
        I2c* bus_d = new I2c(I2C_BUS_D);

        InitSingletons(bus_a, bus_b, bus_c, bus_d);

        // The satellite now runs either the unit tests or the in-orbit
        // configuration for memory reasons.
        // To swap between the two configurations:
        // - Right click on the project (MSP)
        // - Build Configurations
        // - Set Active
        // - Choose Orbit or TIRTOS Build

#if defined TEST_CONFIGURATION
        RunUnitTests();
#elif defined ORBIT_CONFIGURATION
        InitStateManagement();
        if (hil_enabled) InitDataDashboard();
        TaskHolder* pre_deployment_magnetometer_poller_task =
            InitPreDeploymentMagnetometerPoller();

        // TODO(akremor): We should add a force-enable based on number of
        // reboots feature In case the satellite gets stuck in a boot loop or
        // similar, we don't want the timers to be operating each time
        Log_info0("Starting beacon delay timer");
        SatelliteTimeSource::RealTimeWait(kBeaconDelaySeconds);
        Log_info0("Beacon delay timer finished");

        InitRadioListener();
        Log_info0("Radio receiver started");
        InitPayloadProcessor();
        Log_info0("Payload processor started");

        SatelliteTimeSource::RealTimeWait(kAntennaDelaySeconds);
        Log_info0("Antenna deploying, can take awhile");
        DeployAntenna();
        Semaphore_post(RunnablePreDeploymentMagnetometerPoller::
                           kill_task_on_orientation_control_begin_semaphore);
        InitBeacon();
        Log_info0("Beacon started");

        InitOrientationControl();
        Log_info0("Orientation control started");

        InitSystemHealthCheck();
        Log_info0("System healthcheck started");

        Log_info0("System start up complete");
        // TODO(rskew): Ensure all relevant resources are deleted by the pre-mag
        // task before we delete taskholder We may not want to enable this as it
        // makes memory verification harder. If we only ever alloc once and
        // never dealloc it can be easier to determine if we do or do not have
        // sufficient run-time memory.
        // pre_deployment_magnetometer_poller_task->~TaskHolder();
#else
#error "No configuration (orbit/test) defined."
#endif
    } catch (etl::exception e) {
        System_printf("EXCEPTION OCCURRED\n");
        System_printf("File: %s, line %d\n", e.file_name(), e.line_number());
        std::string message = e.what();
        System_printf("Message: %s\n", message.c_str());

        System_flush();
    }
}
