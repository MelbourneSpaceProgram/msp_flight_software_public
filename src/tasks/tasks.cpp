#include <src/debug_interface/runnable_debug_stream.h>
#include <src/tasks/task_holder.h>
#include <src/tasks/tasks.h>

#include <src/telecomms/lithium.h>
#include <src/telecomms/runnable_lithium_listener.h>
#include <stdio.h>

#include <src/system/state_manager.h>
#include <src/system/tasks/runnable_state_management.h>

void TasksInit() {
    TaskHolder* debug_task =
        new TaskHolder(4096, "DebugStream", 6, new RunnableDebugStream());
    debug_task->Init();

    // State Machine initialisation.
    StateManager* state_manager = StateManager::GetStateManager();
    state_manager->CreateStateMachines();

    RunnableStateManagement* runnable_state_management =
        new RunnableStateManagement();

    // TODO(rskew) review priority
    TaskHolder* state_management_task =
        new TaskHolder(1024, "StateManagement", 8, runnable_state_management);

    state_management_task->Init();
}
