#ifndef SRC_SYSTEM_SYSTEM_STATE_MACHINE_H_
#define SRC_SYSTEM_SYSTEM_STATE_MACHINE_H_

#include <src/system/state_definitions.h>
#include <src/system/state_machine.h>

class StateManager;

class SystemStateMachine : public StateMachine {
   public:
    virtual void CheckUpstreamStates() = 0;

   protected:
    SystemStateMachine(StateManager* state_manager, StateId initial_state);
};

#endif  //  SRC_SYSTEM_SYSTEM_STATE_MACHINE_H_
