#ifndef SRC_SYSTEM_SYSTEM_STATE_MACHINES_TELECOMS_STATE_MACHINE_H_
#define SRC_SYSTEM_SYSTEM_STATE_MACHINES_TELECOMS_STATE_MACHINE_H_

#include <src/system/system_state_machine.h>

class StateManager;
class PowerStateMachine;
class TelecomsTempStateMachine;

class TelecomsStateMachine : public SystemStateMachine {
   public:
    TelecomsStateMachine(StateManager* state_manager,
                         PowerStateMachine* power_state_machine,
                         TelecomsTempStateMachine* telecoms_temp_state_machine);
    void CheckUpstreamStates();

   private:
    PowerStateMachine* power_state_machine;
    TelecomsTempStateMachine* telecoms_temp_state_machine;
};

#endif  //  SRC_SYSTEM_SYSTEM_STATE_MACHINES_TELECOMS_STATE_MACHINE_H_
