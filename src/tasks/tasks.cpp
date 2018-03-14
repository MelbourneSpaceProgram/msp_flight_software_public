#include <src/system/state_manager.h>
#include <src/system/tasks/runnable_state_management.h>
#include <src/tasks/task_holder.h>
#include <src/tasks/tasks.h>

void TasksInit() {
    // TODO(rskew) review priority
    TaskHolder* state_management_task = new TaskHolder(
        1024, "StateManagement", 8, new RunnableStateManagement());
    state_management_task->Init();
}
