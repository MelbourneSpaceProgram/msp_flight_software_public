#include <Board.h>
#include <src/debug_interface/debug_stream.h>
#include <src/i2c/i2c.h>
#include <src/init/init.h>
#include <src/init/post_bios_initialiser.h>
#include <src/init/test_initialiser.h>
#include <src/tasks/task_holder.h>
#include <src/tasks/tasks.h>
#include <src/telecomms/antenna.h>

PostBiosInitialiser::PostBiosInitialiser() {}

fnptr PostBiosInitialiser::GetRunnablePointer() {
    return &PostBiosInitialiser::PostBiosInit;
}

void PostBiosInitialiser::PostBiosInit() {
    // TODO(dingbenjamin): Init var length array pool
    DebugStream::GetInstance();  // Initialise Singleton in thread safe manner
    I2c *bus = new I2c(I2cConfiguration(), Board_I2C0);
    Antenna *antenna = Antenna::GetAntenna();
    antenna->InitAntenna(bus);

    TaskHolder *test_task =
        new TaskHolder(4096, "Unit Tests", 7, new TestInitialiser());
    test_task->Init();
    TasksInit();
}
