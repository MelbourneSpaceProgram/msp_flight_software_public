#include <src/system/sensor_state_machines/detumbled_state_machine.h>
#include <src/system/sensor_state_machines/tle_state_machine.h>
#include <src/system/state_definitions.h>
#include <src/system/state_machine.h>
#include <src/system/system_state_machines/adcs_state_machine.h>
#include <src/system/system_state_machines/power_state_machine.h>

AdcsStateMachine::AdcsStateMachine(
    StateManager* state_manager, PowerStateMachine* power_state_machine,
    TleStateMachine* tle_state_machine,
    DetumbledStateMachine* detumbled_state_machine)
    : SystemStateMachine(state_manager, kAdcsOff),
      power_state_machine(power_state_machine),
      tle_state_machine(tle_state_machine),
      detumbled_state_machine(detumbled_state_machine) {}

void AdcsStateMachine::CheckUpstreamStates() {
    StateId power_state = power_state_machine->GetCurrentState();

    if (power_state == kPowerEverythingOff) {
        SetState(kAdcsOff);
    } else if (power_state == kPowerNominal) {
        SetState(kAdcsNominal);
    } else {
        // This state should cause all unit tests to fail
        SetState(kAdcsNoState);
        etl::exception e("State machine entered an invalid state.", __FILE__,
                         __LINE__);
        throw e;
    }
}
