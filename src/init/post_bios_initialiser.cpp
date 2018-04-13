#include <Board.h>
#include <src/adcs/runnable_orientation_control.h>
#include <src/config/unit_tests.h>
#include <src/data_dashboard/runnable_data_dashboard.h>
#include <src/debug_interface/debug_stream.h>
#include <src/init/init.h>
#include <src/init/post_bios_initialiser.h>
#include <src/init/test_initialiser.h>
#include <src/sensors/i2c_measurable_manager.h>
#include <src/system/state_manager.h>
#include <src/system/tasks/runnable_state_management.h>
#include <src/tasks/task_holder.h>
#include <src/telecomms/antenna.h>
#include <src/telecomms/lithium.h>
#include <src/telecomms/runnable_beacon.h>
#include <src/telecomms/runnable_lithium_listener.h>
#include <src/util/runnable_memory_logger.h>
#include <ti/sysbios/BIOS.h>
#include <ti/sysbios/knl/Semaphore.h>
#include <xdc/runtime/System.h>
#include <xdc/std.h>

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
    TaskHolder* state_management_task = new TaskHolder(
        1024, "StateManagement", 8, new RunnableStateManagement());
    state_management_task->Init();
}

void PostBiosInitialiser::InitBeacon() {
    TaskHolder* beacon_task =
        new TaskHolder(2048, "Beacon", 12, new RunnableBeacon());
    beacon_task->Init();
}

void PostBiosInitialiser::InitDataDashboard() {
    // TODO(rskew) review priority
    TaskHolder* data_dashboard_task =
        new TaskHolder(4096, "DataDashboard", 5, new RunnableDataDashboard());
    data_dashboard_task->Init();
}

void PostBiosInitialiser::InitOrientationControl() {
    // TODO(rskew) review priority
    TaskHolder* orientation_control_task = new TaskHolder(
        4096, "OrientationControl", 7, new RunnableOrientationControl());
    orientation_control_task->Init();
}

void PostBiosInitialiser::InitHardware() { I2c::InitBusses(); }

void PostBiosInitialiser::PostBiosInit() {
    TaskHolder* memory_logger_task =
        new TaskHolder(1024, "MemoryLogger", 13, new RunnableMemoryLogger());
    memory_logger_task->Init();

    try {
        // TODO(dingbenjamin): Init var length array pool

        InitHardware();

        I2c* bus_a = new I2c(I2C_BUS_A);
        I2c* bus_b = new I2c(I2C_BUS_B);
        I2c* bus_c = new I2c(I2C_BUS_C);
        I2c* bus_d = new I2c(I2C_BUS_D);

        InitSingletons(bus_a, bus_b, bus_c, bus_d);
        InitRadioListener();

        RunUnitTests();

        StateManager* state_manager = StateManager::GetStateManager();
        state_manager->CreateStateMachines();

        InitBeacon();
        InitOrientationControl();

        if (hil_enabled) {
            InitDataDashboard();
        }
    } catch (etl::exception e) {
        System_printf("EXCEPTION OCCURRED\n");
        System_printf("File: %s, line %d\n", e.file_name(), e.line_number());
        std::string message = e.what();
        System_printf("Message: %s\n", message.c_str());

        System_flush();
    }
}
