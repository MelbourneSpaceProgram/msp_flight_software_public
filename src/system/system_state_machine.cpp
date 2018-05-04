#include <src/system/system_state_machine.h>
#include <src/system/state_manager.h>
#include <src/system/state_machine.h>
#include <src/system/state_definitions.h>

SystemStateMachine::SystemStateMachine(StateManager* state_manager,
                                       StateId initial_state)
    : StateMachine(state_manager, initial_state) {}
