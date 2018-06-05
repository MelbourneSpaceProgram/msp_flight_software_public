#ifndef SRC_SYSTEM_SYSTEM_STATE_MACHINES_POWER_STATE_MACHINE_H_
#define SRC_SYSTEM_SYSTEM_STATE_MACHINES_POWER_STATE_MACHINE_H_

#include <src/system/system_state_machine.h>

class BatteryChargeStateMachine;
class BatteryTempStateMachine;

class PowerStateMachine : public SystemStateMachine {
   public:
    PowerStateMachine(StateManager* state_manager,
                      BatteryChargeStateMachine* battery_charge_state_machine,
                      BatteryTempStateMachine* battery_temp_state_machine);
    void CheckUpstreamStates();

   private:
    BatteryChargeStateMachine* battery_charge_state_machine;
    BatteryTempStateMachine* battery_temp_state_machine;
};

#endif  //  SRC_SYSTEM_SYSTEM_STATE_MACHINES_POWER_STATE_MACHINE_H_
