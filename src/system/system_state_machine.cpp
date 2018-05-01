#include <src/system/system_state_machine.h>
#include <src/system/state_manager.h>
#include <src/system/state_machine.h>
#include <src/system/state_definitions.h>
#include <ti/sysbios/knl/Mailbox.h>

SystemStateMachine::SystemStateMachine(StateManager* state_manager,
                                       StateId initial_state)
    : StateMachine(state_manager, initial_state) {}

void SystemStateMachine::EnableFunction(FunctionEnableHandle handle_id) {
    bool enabled = true;
    Mailbox_post(state_manager->GetFunctionEnableHandle(handle_id), &enabled,
                 0);
}

void SystemStateMachine::DisableFunction(FunctionEnableHandle handle_id) {
    bool enabled = false;
    Mailbox_post(state_manager->GetFunctionEnableHandle(handle_id), &enabled,
                 0);
}
