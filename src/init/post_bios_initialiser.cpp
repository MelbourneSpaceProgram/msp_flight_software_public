#include <Board.h>
#include <src/config/unit_tests.h>
#include <src/debug_interface/debug_stream.h>
#include <src/i2c/i2c.h>
#include <src/init/init.h>
#include <src/init/post_bios_initialiser.h>
#include <src/init/test_initialiser.h>
#include <src/tasks/task_holder.h>
#include <src/tasks/tasks.h>
#include <src/telecomms/antenna.h>
#include <src/telecomms/lithium.h>
#include <src/telecomms/runnable_lithium_listener.h>
#include <ti/sysbios/BIOS.h>
#include <ti/sysbios/knl/Semaphore.h>

PostBiosInitialiser::PostBiosInitialiser() {}

fnptr PostBiosInitialiser::GetRunnablePointer() {
    return &PostBiosInitialiser::PostBiosInit;
}

void PostBiosInitialiser::PostBiosInit() {
    // TODO(dingbenjamin): Init var length array pool
    // Initialise Singletons in thread safe manner
    DebugStream::GetInstance();
    Antenna::GetAntenna();
    Lithium::GetInstance();

    I2c *bus = new I2c(Board_I2C0);
    Antenna::GetAntenna()->InitAntenna(bus);

    Semaphore_Params sem_params;
    Semaphore_Params_init(&sem_params);
    Semaphore_Handle test_complete = Semaphore_create(0, &sem_params, NULL);
    TestInitialiser::GetInstance()->InitSemaphore(test_complete);
    TaskHolder *test_task =
        new TaskHolder(4096, "Unit Tests", 7, TestInitialiser::GetInstance());
    TaskHolder *radio_listener = new TaskHolder(1024, "RadioListener", 11,
                                                new RunnableLithiumListener());
    radio_listener->Init();
    test_task->Init();
    Semaphore_pend(test_complete, BIOS_WAIT_FOREVER);

    TasksInit();
}
