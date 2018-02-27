#include <src/system/state_machine.h>
#include <src/system/state_manager.h>

StateMachine::StateMachine(StateManager* state_manager, StateId initial_state)
    : state_manager(state_manager),
      current_state(initial_state),
      locked(false) {}

StateId StateMachine::GetCurrentState() const { return current_state; }

StateManager* StateMachine::GetStateManager() const { return state_manager; }

bool StateMachine::IsLocked() const { return locked; }

void StateMachine::SetState(StateId new_state) {
    // TODO(rskew) validate state argument
    if (!locked) {
        current_state = new_state;
    }
}

void StateMachine::OverrideState(StateId new_state) {
    // TODO(rskew) not threadsafe
    locked = true;
    current_state = new_state;
}
