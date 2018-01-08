#include <src/init/init.h>
#include <src/init/post_bios_initialiser.h>
#include <src/init/test_initialiser.h>
#include <src/tasks/task_holder.h>
#include <src/tasks/tasks.h>

PostBiosInitialiser::PostBiosInitialiser() {}

fnptr PostBiosInitialiser::GetRunnablePointer() {
    return &PostBiosInitialiser::PostBiosInit;
}

void PostBiosInitialiser::PostBiosInit() {
    // TODO(dingbenjamin): Init var length array pool
    TaskHolder *test_task =
        new TaskHolder(4096, "Unit Tests", 7, new TestInitialiser());
    test_task->Init();
    TasksInit();
}
