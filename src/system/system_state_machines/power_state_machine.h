#ifndef SRC_SYSTEM_SYSTEM_STATE_MACHINES_POWER_STATE_MACHINE_H_
#define SRC_SYSTEM_SYSTEM_STATE_MACHINES_POWER_STATE_MACHINE_H_

#include <src/system/system_state_machine.h>
#include <src/system/sensor_state_machines/battery_charge_state_machine.h>
#include <src/system/sensor_state_machines/battery_temp_state_machine.h>

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
