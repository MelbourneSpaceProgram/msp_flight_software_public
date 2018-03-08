#ifndef SRC_SYSTEM_STATE_MACHINE_H_
#define SRC_SYSTEM_STATE_MACHINE_H_

#include <src/system/state_definitions.h>

class StateManager;

class StateMachine {
   public:
    StateId GetCurrentState() const;
    StateManager* GetStateManager() const;
    bool IsLocked() const;
    void OverrideState(StateId new_state);

   protected:
    StateMachine(StateManager* state_manager, StateId initial_state);
    void SetState(StateId new_state);

   private:
    StateId current_state;
    StateManager* state_manager;
    bool locked;
};

#endif  // SRC_SYSTEM_STATE_MACHINE_H_