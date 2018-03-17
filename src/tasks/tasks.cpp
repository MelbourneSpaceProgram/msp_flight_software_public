#include <src/system/state_manager.h>
#include <src/system/tasks/runnable_state_management.h>
#include <src/tasks/task_holder.h>
#include <src/tasks/tasks.h>

#include <src/data_dashboard/runnable_data_dashboard.h>

void TasksInit() {
    // TODO(rskew) review priority
    TaskHolder* state_management_task = new TaskHolder(
        1024, "StateManagement", 8, new RunnableStateManagement());
    state_management_task->Init();

    // TODO(rskew) review priority
    TaskHolder* data_dashboard_task = new TaskHolder(
        4096, "DataDashboard", 5, new RunnableDataDashboard());
    data_dashboard_task->Init();
}
