#include <src/debug_interface/runnable_debug_stream.h>
#include <src/tasks/task_holder.h>
#include <src/tasks/tasks.h>
#include <src/telecomms/runnable_lithium.h>

void TasksInit() {
    TaskHolder *debug_task =
        new TaskHolder(4096, "DebugStream", 6, new RunnableDebugStream());
    debug_task->Init();
}
