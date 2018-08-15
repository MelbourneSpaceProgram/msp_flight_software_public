#include <src/system/runnable_state_management.h>
#include <src/system/state_definitions.h>
#include <src/system/state_machine.h>
#include <src/system/state_manager.h>
#include <src/util/system_watchdog.h>

RunnableStateManagement::RunnableStateManagement() {}

fnptr RunnableStateManagement::GetRunnablePointer() {
    return &RunnableStateManagement::StateManagement;
}

void RunnableStateManagement::StateManagement() {
    StateManager* state_manager = StateManager::GetStateManager();

    while (1) {
        // Task will block when there are no state changes. As this is a
        // high priority thread this task will not be preempted.
        state_manager->ProcessStateChanges();
        SystemWatchdog::ResetTimer();
    }
}
