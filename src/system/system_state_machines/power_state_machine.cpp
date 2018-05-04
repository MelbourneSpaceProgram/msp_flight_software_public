#include <src/system/sensor_state_machines/battery_charge_state_machine.h>
#include <src/system/sensor_state_machines/battery_temp_state_machine.h>
#include <src/system/state_definitions.h>
#include <src/system/state_machine.h>
#include <src/system/state_manager.h>
#include <src/system/system_state_machines/power_state_machine.h>

PowerStateMachine::PowerStateMachine(
    StateManager* state_manager,
    BatteryChargeStateMachine* battery_charge_state_machine,
    BatteryTempStateMachine* battery_temp_state_machine)
    : SystemStateMachine(state_manager, kPowerLimited),
      battery_charge_state_machine(battery_charge_state_machine),
      battery_temp_state_machine(battery_temp_state_machine) {}

void PowerStateMachine::CheckUpstreamStates() {
    StateId battery_charge = battery_charge_state_machine->GetCurrentState();
    StateId battery_temp = battery_temp_state_machine->GetCurrentState();

    if (battery_temp == kBatteryTempCriticalHigh) {
        SetState(kPowerEverythingOff);

    }  else if (battery_temp == kBatteryTempNominal) {
        SetState(kPowerNominal);

    } else {
        // This state should cause all unit tests to fail
        SetState(kPowerNoState);
        // TODO(rskew) log major error
        etl::exception e("State machine entered an invalid state.", __FILE__,
                         __LINE__);
        throw e;
    }
}
