#include <Board.h>
#include <src/adcs/magnetorquer_control.h>
#include <src/adcs/runnable_orientation_control.h>
#include <src/adcs/runnable_pre_deployment_magnetometer_poller.h>
#include <src/config/board_definitions.h>
#include <src/config/unit_tests.h>
#include <src/data_dashboard/runnable_data_dashboard.h>
#include <src/database/eeprom.h>
#include <src/database/sd_card.h>
#include <src/debug_interface/debug_stream.h>
#include <src/init/init.h>
#include <src/init/post_bios_initialiser.h>
#include <src/init/test_initialiser.h>
#include <src/payload_processor/runnable_payload_processor.h>
#include <src/sensors/i2c_measurable_manager.h>
#include <src/system/state_manager.h>
#include <src/system/tasks/runnable_state_management.h>s
#include <src/telecomms/antenna.h>
#include <src/telecomms/lithium.h>
#include <src/telecomms/runnable_beacon.h>
#include <src/telecomms/runnable_lithium_listener.h>
#include <src/util/runnable_memory_logger.h>
#include <src/util/runnable_time_source.h>
#include <src/util/task_utils.h>
#include <ti/sysbios/BIOS.h>
#include <ti/sysbios/knl/Semaphore.h>
#include <xdc/runtime/Error.h>
#include <xdc/runtime/Log.h>
#include <xdc/std.h>
#include <string>

PostBiosInitialiser::PostBiosInitialiser() {}

fnptr PostBiosInitialiser::GetRunnablePointer() {
    return &PostBiosInitialiser::PostBiosInit;
}

void PostBiosInitialiser::InitSingletons(I2c* bus_a, I2c* bus_b, I2c* bus_c,
                                         I2c* bus_d) {
    DebugStream::GetInstance();
    Antenna::GetAntenna()->InitAntenna(bus_d);
    Lithium::GetInstance();
    StateManager::GetStateManager()->CreateStateMachines();
    I2cMeasurableManager::GetInstance()->Init(bus_a, bus_b, bus_c, bus_d);
}

void PostBiosInitialiser::InitRadioListener() {
    TaskHolder* radio_listener = new TaskHolder(1024, "RadioListener", 11,
                                                new RunnableLithiumListener());
    radio_listener->Init();
}

void PostBiosInitialiser::RunUnitTests() {
    Semaphore_Params sem_params;
    Semaphore_Params_init(&sem_params);
    Semaphore_Handle test_complete = Semaphore_create(0, &sem_params, NULL);
    TestInitialiser::GetInstance()->InitSemaphore(test_complete);
    TaskHolder* test_task =
        new TaskHolder(11000, "Unit Tests", 7, TestInitialiser::GetInstance());
    test_task->Init();
    Semaphore_pend(test_complete, BIOS_WAIT_FOREVER);
    delete test_task;
}

void PostBiosInitialiser::InitStateManagement() {
    // TODO(rskew) review priority
    StateManager* state_manager = StateManager::GetStateManager();
    state_manager->CreateStateMachines();

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

void PostBiosInitialiser::DeployAntenna() {
    Antenna* antenna = Antenna::GetAntenna();
    if (!antenna->IsDoorsOpen()) antenna->SafeDeploy();
    if (!antenna->IsDoorsOpen()) antenna->ForceDeploy();
    if (!antenna->IsDoorsOpen()) {
        Log_error0("Antenna failed to deploy");
    }
}

void PostBiosInitialiser::InitHardware() {
    I2c::InitBusses();
    Eeprom::Init();
    MagnetorquerControl::Initialize();
}

void PostBiosInitialiser::DeploymentWait(uint16_t delay) {
    // The deployment wait acts as a timer counting for `delay` minutes,
    // starting from the time the `DeploymentWait` call is made.
    // It is a blocking wait.

    I2cMeasurableManager* manager = I2cMeasurableManager::GetInstance();

    RTime reading = manager->ReadI2cMeasurable<RTime>(kCdhRtc, 0);
    time_t init_time = Rtc::RTimeToEpoch(reading);
    time_t cur_time = init_time;

    while ((cur_time - init_time) / kSecsInMin < delay) {
        reading = manager->ReadI2cMeasurable<RTime>(kCdhRtc, 0);
        if (Rtc::ValidTime(reading)) {
            cur_time = Rtc::RTimeToEpoch(reading);
        }
        TaskUtils::SleepMilli(kDelayCheckInterval);
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
    InitMemoryLogger();

    try {
        // TODO(dingbenjamin): Init var length array pool

//        InitHardware();
//        InitTimeSource();  // Relies on I2C so needs to be post InitHardware()

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
        DeploymentWait(kBeaconDelayMins);
        InitRadioListener();
        DeploymentWait(kAntennaDelayMins);
        DeployAntenna();
        Semaphore_post(RunnablePreDeploymentMagnetometerPoller::
                           kill_task_on_orientation_control_begin_semaphore);
        InitBeacon();
        InitPayloadProcessor();
        InitOrientationControl();
        // TODO(rskew): Debug what needs to be passed in to Task_delete
        //Task_delete(pre_deployment_magnetometer_poller_task);
#else
        System_printf("No configuration defined. Not doing anything");
#endif
    } catch (etl::exception e) {
        System_printf("EXCEPTION OCCURRED\n");
        System_printf("File: %s, line %d\n", e.file_name(), e.line_number());
        std::string message = e.what();
        System_printf("Message: %s\n", message.c_str());

        System_flush();
    }
}
