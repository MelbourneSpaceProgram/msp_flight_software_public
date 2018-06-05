#include <src/config/unit_tests.h>
#include <src/system/sensor_state_machines/detumbled_state_machine.h>
#include <src/system/sensor_state_machines/telecoms_temp_state_machine.h>
#include <src/system/sensor_state_machines/tle_state_machine.h>
#include <src/system/state_definitions.h>
#include <src/system/state_machine.h>
#include <src/system/state_manager.h>
#include <src/system/system_state_machines/adcs_state_machine.h>
#include <src/system/system_state_machines/power_state_machine.h>
#include <src/system/system_state_machines/telecoms_state_machine.h>
#include <test_runners/unity.h>

static const uint8_t kNumBatteryChargeStates = 3;
static const uint8_t kNumBatteryTempStates = 3;
static const uint8_t kNumTelecomsTempStates = 2;
static const uint8_t kNumTleStates = 2;
static const uint8_t kNumDetumbledStates = 2;
static const uint8_t kNumPowerStates = 3;

StateId battery_charge_states[kNumBatteryChargeStates] = {
    kBatteryChargeCriticalLow, kBatteryChargeLow, kBatteryChargeNominal};

StateId telecoms_temp_states[kNumTelecomsTempStates] = {
    kTelecomsTempNominal, kTelecomsTempCriticalHigh};

StateId tle_states[kNumTleStates] = {kTleInvalid, kTleValid};

StateId detumbled_states[kNumDetumbledStates] = {kDetumbledFalse,
                                                 kDetumbledTrue};

StateId power_states[kNumPowerStates] = {kPowerEverythingOff, kPowerLimited,
                                         kPowerNominal};

void TestTelecomsStateLogic() {
    PowerStateMachine power_state_machine(NULL, NULL, NULL);
    TelecomsTempStateMachine telecoms_temp_state_machine(NULL);
    TelecomsStateMachine telecoms_state_machine(NULL, &power_state_machine,
                                                &telecoms_temp_state_machine);

    // Check aaaaaaaall the permutations
    for (uint8_t power_index = 0; power_index < kNumPowerStates;
         power_index++) {
        for (uint8_t telecoms_temp_index = 0;
             telecoms_temp_index < kNumTelecomsTempStates;
             telecoms_temp_index++) {
            power_state_machine.OverrideState(power_states[power_index]);

            telecoms_temp_state_machine.OverrideState(
                telecoms_temp_states[telecoms_temp_index]);

            telecoms_state_machine.CheckUpstreamStates();
            StateId current_telecoms_state =
                telecoms_state_machine.GetCurrentState();

            if (power_states[power_index] == kPowerEverythingOff) {
                TEST_ASSERT_EQUAL_INT(kTelecoms0, current_telecoms_state);
            } else if (power_states[power_index] == kPowerLimited) {
                TEST_ASSERT_TRUE(kTelecoms0 == current_telecoms_state ||
                                 kTelecoms1 == current_telecoms_state);
            }

            if (telecoms_temp_states[telecoms_temp_index] ==
                kTelecomsTempCriticalHigh) {
                TEST_ASSERT_EQUAL_INT(kTelecoms0, current_telecoms_state);
            }

            if (power_states[power_index] == kPowerNominal &&
                telecoms_temp_states[telecoms_temp_index] ==
                    kTelecomsTempNominal) {
                TEST_ASSERT_EQUAL_INT(kTelecoms2, current_telecoms_state);
            }
        }
    }
}

void TestAdcsStateLogic() {
    PowerStateMachine power_state_machine(NULL, NULL, NULL);
    TleStateMachine tle_state_machine(NULL);
    DetumbledStateMachine detumbled_state_machine(NULL);
    AdcsStateMachine adcs_state_machine(NULL, &power_state_machine,
                                        &tle_state_machine,
                                        &detumbled_state_machine);

    // Check aaaaaaaall the permutations
    for (uint8_t power_index = 0; power_index < kNumPowerStates;
         power_index++) {
        for (uint8_t detumbled_index = 0; detumbled_index < kNumDetumbledStates;
             detumbled_index++) {
            for (uint8_t tle_index = 0; tle_index < kNumTleStates;
                 tle_index++) {
                power_state_machine.OverrideState(power_states[power_index]);

                tle_state_machine.OverrideState(tle_states[tle_index]);

                detumbled_state_machine.OverrideState(
                    detumbled_states[detumbled_index]);

                adcs_state_machine.CheckUpstreamStates();
                StateId current_adcs_state =
                    adcs_state_machine.GetCurrentState();

                if (power_states[power_index] == kPowerEverythingOff) {
                    TEST_ASSERT_EQUAL_INT(kAdcsOff, current_adcs_state);
                } else if (power_states[power_index] == kPowerLimited) {
                    TEST_ASSERT_EQUAL_INT(kAdcsOff, current_adcs_state);
                }

                if (detumbled_states[detumbled_index] == kDetumbledFalse) {
                    TEST_ASSERT_TRUE(kAdcsOff == current_adcs_state ||
                                     kAdcsDetumbling == current_adcs_state);
                }

                if (tle_states[tle_index] == kTleInvalid) {
                    TEST_ASSERT_TRUE(kAdcsOff == current_adcs_state ||
                                     kAdcsDetumbling == current_adcs_state ||
                                     kAdcsNadirPointing == current_adcs_state);
                }

                if (power_states[power_index] == kPowerNominal &&
                    detumbled_states[detumbled_index] == kDetumbledTrue &&
                    tle_states[tle_index] == kTleValid) {
                    TEST_ASSERT_EQUAL_INT(kAdcsNominal, current_adcs_state);
                }
            }
        }
    }
}
