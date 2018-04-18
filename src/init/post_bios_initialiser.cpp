#include <Board.h>
#include <src/adcs/runnable_orientation_control.h>
#include <src/config/unit_tests.h>
#include <src/data_dashboard/runnable_data_dashboard.h>
#include <src/database/eeprom.h>
#include <src/debug_interface/debug_stream.h>
#include <src/init/init.h>
#include <src/init/post_bios_initialiser.h>
#include <src/init/test_initialiser.h>
#include <src/payload_processor/runnable_payload_processor.h>
#include <src/sensors/i2c_measurable_manager.h>
#include <src/system/state_manager.h>
#include <src/system/tasks/runnable_state_management.h>
#include <src/tasks/task_holder.h>
#include <src/telecomms/antenna.h>
#include <src/telecomms/lithium.h>
#include <src/telecomms/runnable_beacon.h>
#include <src/telecomms/runnable_lithium_listener.h>
#include <src/util/runnable_memory_logger.h>
#include <src/util/runnable_time_source.h>
#include <src/util/task_utils.h>
#include <ti/sysbios/BIOS.h>
#include <ti/sysbios/hal/Timer.h>
#include <ti/sysbios/knl/Semaphore.h>
#include <xdc/runtime/Error.h>
#include <xdc/runtime/Log.h>
#include <xdc/runtime/System.h>
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
        new TaskHolder(4096, "Unit Tests", 7, TestInitialiser::GetInstance());
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

void PostBiosInitialiser::OrientationControlTimerISR(UArg timer_semaphore) {
    Semaphore_post((Semaphore_Handle)timer_semaphore);
}

void PostBiosInitialiser::InitOrientationControl() {
    Timer_Handle orientation_control_timer;
    Timer_Params timerParams;
    Semaphore_Params orientation_control_timer_semaphore_params;
    Semaphore_Handle orientation_control_timer_semaphore;
    Semaphore_Params_init(&orientation_control_timer_semaphore_params);
    orientation_control_timer_semaphore =
        Semaphore_create(0, &orientation_control_timer_semaphore_params, NULL);
    Timer_Params_init(&timerParams);
    Error_init(NULL);
    timerParams.period = RunnableOrientationControl::kControlLoopPeriodMicros;
    timerParams.arg = (UArg)orientation_control_timer_semaphore;
    // TODO (rskew) use a specific timer
    orientation_control_timer =
        Timer_create(Timer_ANY, PostBiosInitialiser::OrientationControlTimerISR,
                     &timerParams, NULL);
    if (orientation_control_timer == NULL) {
        etl::exception e("Timer create failed", __FILE__, __LINE__);
        throw e;
    }

    RunnableOrientationControl* runnable_orientation_control =
        new RunnableOrientationControl();
    runnable_orientation_control->SetTimerSemaphore(
        orientation_control_timer_semaphore);

    // TODO(rskew) review priority
    TaskHolder* orientation_control_task = new TaskHolder(
        4096, "OrientationControl", 7, runnable_orientation_control);
    orientation_control_task->Init();
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
}

void PostBiosInitialiser::DeploymentWait() {
    I2cMeasurableManager* manager = I2cMeasurableManager::GetInstance();
    RTime time = manager->ReadI2cMeasurable<RTime>(kCdhRtc, 0);

    // TODO(dingbenjamin): Make a function for this time check
    while (time.min < 30 && time.hour < 1 && time.date < 2 && time.month < 2) {
        RTime reading = manager->ReadI2cMeasurable<RTime>(kCdhRtc, 0);
        if (Rtc::ValidTime(reading)) {
            time = reading;
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
    TaskHolder* time_source_task =
        new TaskHolder(1024, "TimeSource", 13, new RunnableTimeSource());
    time_source_task->Init();
}

void PostBiosInitialiser::PostBiosInit() {
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

        RunUnitTests();
        InitStateManagement();
        if (hil_enabled) InitDataDashboard();

        DeploymentWait();

        // TODO(dingbenjamin): Deploy antenna conditional on power usage
        InitRadioListener();
        DeployAntenna();
        InitBeacon();
        InitPayloadProcessor();
        InitOrientationControl();
    } catch (etl::exception e) {
        System_printf("EXCEPTION OCCURRED\n");
        System_printf("File: %s, line %d\n", e.file_name(), e.line_number());
        std::string message = e.what();
        System_printf("Message: %s\n", message.c_str());

        System_flush();
    }
}
