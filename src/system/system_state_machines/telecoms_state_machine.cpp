#include <src/system/sensor_state_machines/telecoms_temp_state_machine.h>
#include <src/system/state_definitions.h>
#include <src/system/state_manager.h>
#include <src/system/system_state_machines/power_state_machine.h>
#include <src/system/system_state_machines/telecoms_state_machine.h>

TelecomsStateMachine::TelecomsStateMachine(
    StateManager* state_manager, PowerStateMachine* power_state_machine,
    TelecomsTempStateMachine* telecoms_temp_state_machine)
    : SystemStateMachine(state_manager, kTelecoms0),
      power_state_machine(power_state_machine),
      telecoms_temp_state_machine(telecoms_temp_state_machine) {}

void TelecomsStateMachine::CheckUpstreamStates() {
    StateId power_state = power_state_machine->GetCurrentState();
    StateId telecoms_temp = telecoms_temp_state_machine->GetCurrentState();

    if (power_state == kPowerEverythingOff ||
        telecoms_temp == kTelecomsTempCriticalHigh) {
        SetState(kTelecoms0);

    } else if (telecoms_temp == kTelecomsTempNominal &&
               power_state == kPowerLimited) {
        SetState(kTelecoms1);

    } else if (telecoms_temp == kTelecomsTempNominal &&
               power_state == kPowerNominal) {
        SetState(kTelecoms2);

    } else {
        // This state should cause all unit tests to fail
        SetState(kTelecomsNoState);
        // TODO(rskew) log major error
    }
}
