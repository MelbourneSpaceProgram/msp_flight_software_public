#include <src/config/unit_tests.h>
#include <src/sensors/specific_sensors/test_i2c_sensor.h>
#include <src/system/sensor_state_machines/battery_charge_state_machine.h>
#include <src/system/sensor_state_machines/battery_temp_state_machine.h>
#include <src/system/sensor_state_machines/telecoms_temp_state_machine.h>
#include <src/system/state_definitions.h>
#include <src/system/state_manager.h>
#include <test_runners/sensor_state_logic_tests.h>
#include <test_runners/unity.h>
#include <string>

static const float kTwoEclipseCapacity = 0.2;
static const float kThreePassCapacity = 0.4;
static const float kHysteresis = 0.02;

static const uint8_t kTempBatteryOperationalLow = 5;
static const uint8_t kTempBatteryOperationalHigh = 40;
static const uint8_t kHysteresisTemp = 1;

static const uint8_t kTempTelecomsOperationalMax = 80;

StateId BatteryChargeStateCheck(double soc, StateId current_state) {
    switch (current_state) {
        case kBatteryChargeCriticalLow:
            if (soc > kTwoEclipseCapacity + kHysteresis) {
                return (kBatteryChargeLow);
            }
            break;
        case kBatteryChargeLow:
            if (soc < kTwoEclipseCapacity) {
                return (kBatteryChargeCriticalLow);
            } else if (soc > kThreePassCapacity + kHysteresis) {
                return (kBatteryChargeNominal);
            }
            break;
        case kBatteryChargeNominal:
            if (soc < kThreePassCapacity) {
                return (kBatteryChargeLow);
            }
            break;
    }
    return current_state;
}

StateId BatteryTempStateCheck(double temp, StateId current_state) {
    switch (current_state) {
        case kBatteryTempCriticalLow:
            if (temp > kTempBatteryOperationalLow + kHysteresisTemp) {
                return (kBatteryTempNominal);
            }
            break;
        case kBatteryTempNominal:
            if (temp < kTempBatteryOperationalLow) {
                return (kBatteryTempCriticalLow);
            } else if (temp > kTempBatteryOperationalHigh + kHysteresisTemp) {
                return (kBatteryTempCriticalHigh);
            }
            break;
        case kBatteryTempCriticalHigh:
            if (temp < kTempBatteryOperationalHigh) {
                return (kBatteryTempNominal);
            }
            break;
    }
    return current_state;
}

StateId TelecomsTempStateCheck(double temp, StateId current_state) {
    switch (current_state) {
        case kTelecomsTempNominal:
            if (temp > kTempTelecomsOperationalMax + kHysteresisTemp) {
                return (kTelecomsTempCriticalHigh);
            }
            break;
        case kTelecomsTempCriticalHigh:
            if (temp < kTempTelecomsOperationalMax) {
                return (kTelecomsTempNominal);
            }
            break;
    }
    return current_state;
}

void TestBatteryChargeStateFlow(void) {
    StateManager* state_manager = StateManager::GetStateManager();
    BatteryChargeStateMachine battery_charge_sm(state_manager);

    TestI2cSensor battery_soc_sensor("battery_soc_sensor");
    battery_charge_sm.RegisterWithSensor(&battery_soc_sensor);

    uint8_t MAX_SOC = 1;
    uint8_t MIN_SOC = 0.9;
    double dummy_soc_reading = MIN_SOC;
    StateId state_before_reading;
    while (dummy_soc_reading < MAX_SOC) {
        state_before_reading = battery_charge_sm.GetCurrentState();
        battery_soc_sensor.SetDummySensorData(dummy_soc_reading);
        battery_soc_sensor.TakeReading();

        TEST_ASSERT_EQUAL_INT(
            BatteryChargeStateCheck(dummy_soc_reading, state_before_reading),
            battery_charge_sm.GetCurrentState());
        dummy_soc_reading += 0.01;
    }

    while (dummy_soc_reading > MIN_SOC) {
        state_before_reading = battery_charge_sm.GetCurrentState();
        battery_soc_sensor.SetDummySensorData(dummy_soc_reading);
        battery_soc_sensor.TakeReading();

        TEST_ASSERT_EQUAL_INT(
            BatteryChargeStateCheck(dummy_soc_reading, state_before_reading),
            battery_charge_sm.GetCurrentState());
        dummy_soc_reading -= 0.01;
    }
}

void TestBatteryTempStateFlow(void) {
    StateManager* state_manager = StateManager::GetStateManager();
    BatteryTempStateMachine battery_temp_sm(state_manager);

    TestI2cSensor battery_temp_sensor("battery_temp_sensor");
    battery_temp_sm.RegisterWithSensor(&battery_temp_sensor);

    uint8_t MAX_TEMP = 50;
    uint8_t MIN_TEMP = 0;
    double dummy_temp_reading = MIN_TEMP;
    StateId state_before_reading;
    while (dummy_temp_reading < MAX_TEMP) {
        state_before_reading = battery_temp_sm.GetCurrentState();
        battery_temp_sensor.SetDummySensorData(dummy_temp_reading);
        battery_temp_sensor.TakeReading();

        TEST_ASSERT_EQUAL_INT(
            BatteryTempStateCheck(dummy_temp_reading, state_before_reading),
            battery_temp_sm.GetCurrentState());
        dummy_temp_reading += 1;
    }

    while (dummy_temp_reading > MIN_TEMP) {
        state_before_reading = battery_temp_sm.GetCurrentState();
        battery_temp_sensor.SetDummySensorData(dummy_temp_reading);
        battery_temp_sensor.TakeReading();

        TEST_ASSERT_EQUAL_INT(
            BatteryTempStateCheck(dummy_temp_reading, state_before_reading),
            battery_temp_sm.GetCurrentState());
        dummy_temp_reading -= 1;
    }
}

void TestTelecomsTempStateFlow(void) {
    StateManager* state_manager = StateManager::GetStateManager();
    TelecomsTempStateMachine telecoms_temp_sm(state_manager);

    TestI2cSensor telecoms_temp_sensor("telecoms_temp_sensor");
    telecoms_temp_sm.RegisterWithSensor(&telecoms_temp_sensor);

    uint8_t MAX_TEMP = kTempTelecomsOperationalMax + 10;
    uint8_t MIN_TEMP = 78;
    double dummy_temp_reading = MIN_TEMP;
    StateId state_before_reading;
    while (dummy_temp_reading < MAX_TEMP) {
        state_before_reading = telecoms_temp_sm.GetCurrentState();
        telecoms_temp_sensor.SetDummySensorData(dummy_temp_reading);
        telecoms_temp_sensor.TakeReading();

        TEST_ASSERT_EQUAL_INT(
            TelecomsTempStateCheck(dummy_temp_reading, state_before_reading),
            telecoms_temp_sm.GetCurrentState());
        dummy_temp_reading += 1;
    }

    while (dummy_temp_reading > MIN_TEMP) {
        state_before_reading = telecoms_temp_sm.GetCurrentState();
        telecoms_temp_sensor.SetDummySensorData(dummy_temp_reading);
        telecoms_temp_sensor.TakeReading();

        TEST_ASSERT_EQUAL_INT(
            TelecomsTempStateCheck(dummy_temp_reading, state_before_reading),
            telecoms_temp_sm.GetCurrentState());
        dummy_temp_reading -= 1;
    }
}
