#include <Board.h>
#include <src/tasks/runnable_debug_stream.h>
#include <src/tasks/task_holder.h>
#include <src/tasks/tasks.h>
#include <xdc/std.h>

void InitTasks() {
    TaskHolder *debug_task =
        new TaskHolder(4096, "DebugStream", 6, new RunnableDebugStream());
    debug_task->Init();
}
