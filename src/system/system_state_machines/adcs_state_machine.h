#ifndef SRC_SYSTEM_SYSTEM_STATE_MACHINES_ADCS_STATE_MACHINE_H_
#define SRC_SYSTEM_SYSTEM_STATE_MACHINES_ADCS_STATE_MACHINE_H_

#include <src/system/system_state_machine.h>

class PowerStateMachine;
class TleStateMachine;
class DetumbledStateMachine;

class AdcsStateMachine : public SystemStateMachine {
   public:
    AdcsStateMachine(StateManager* state_manager,
                     PowerStateMachine* power_state_machine,
                     TleStateMachine* tle_state_machine,
                     DetumbledStateMachine* detumbled_state_machine);
    void CheckUpstreamStates();

   private:
    PowerStateMachine* power_state_machine;
    TleStateMachine* tle_state_machine;
    DetumbledStateMachine* detumbled_state_machine;
};

#endif  //  SRC_SYSTEM_SYSTEM_STATE_MACHINES_ADCS_STATE_MACHINE_H_
