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

static const double kTempBatteryOperationalLow = 5;
static const double kTempBatteryOperationalHigh = 40;
static const double kHysteresisTemp = 1;

static const double kTempTelecomsOperationalMax = 80;

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
    BatteryChargeStateMachine battery_charge_state_machine(state_manager);

    TestI2cSensor battery_soc_sensor("battery_soc_sensor");
    battery_charge_state_machine.RegisterWithSensor(&battery_soc_sensor);

    double kMaxSoc = 1;
    double kMinSoc = 0.9;
    double dummy_soc_reading = kMinSoc;
    StateId state_before_reading;
    while (dummy_soc_reading < kMaxSoc) {
        state_before_reading = battery_charge_state_machine.GetCurrentState();
        battery_soc_sensor.SetDummySensorData(dummy_soc_reading);
        battery_soc_sensor.TakeReading();

        TEST_ASSERT_EQUAL_INT(
            BatteryChargeStateCheck(dummy_soc_reading, state_before_reading),
            battery_charge_state_machine.GetCurrentState());
        dummy_soc_reading += 0.01;
    }

    while (dummy_soc_reading > kMinSoc) {
        state_before_reading = battery_charge_state_machine.GetCurrentState();
        battery_soc_sensor.SetDummySensorData(dummy_soc_reading);
        battery_soc_sensor.TakeReading();

        TEST_ASSERT_EQUAL_INT(
            BatteryChargeStateCheck(dummy_soc_reading, state_before_reading),
            battery_charge_state_machine.GetCurrentState());
        dummy_soc_reading -= 0.01;
    }
}

void TestBatteryTempStateFlow(void) {
    StateManager* state_manager = StateManager::GetStateManager();
    BatteryTempStateMachine battery_temp_state_machine(state_manager);

    TestI2cSensor battery_temp_sensor("battery_temp_sensor");
    battery_temp_state_machine.RegisterWithSensor(&battery_temp_sensor);

    double kMaxTemp = 50;
    double kMinTemp = 0;
    double dummy_temp_reading = kMinTemp;
    StateId state_before_reading;
    while (dummy_temp_reading < kMaxTemp) {
        state_before_reading = battery_temp_state_machine.GetCurrentState();
        battery_temp_sensor.SetDummySensorData(dummy_temp_reading);
        battery_temp_sensor.TakeReading();

        TEST_ASSERT_EQUAL_INT(
            BatteryTempStateCheck(dummy_temp_reading, state_before_reading),
            battery_temp_state_machine.GetCurrentState());
        dummy_temp_reading += 1;
    }

    while (dummy_temp_reading > kMinTemp) {
        state_before_reading = battery_temp_state_machine.GetCurrentState();
        battery_temp_sensor.SetDummySensorData(dummy_temp_reading);
        battery_temp_sensor.TakeReading();

        TEST_ASSERT_EQUAL_INT(
            BatteryTempStateCheck(dummy_temp_reading, state_before_reading),
            battery_temp_state_machine.GetCurrentState());
        dummy_temp_reading -= 1;
    }
}

void TestTelecomsTempStateFlow(void) {
    StateManager* state_manager = StateManager::GetStateManager();
    TelecomsTempStateMachine telecoms_temp_state_machine(state_manager);

    TestI2cSensor telecoms_temp_sensor("telecoms_temp_sensor");
    telecoms_temp_state_machine.RegisterWithSensor(&telecoms_temp_sensor);

    double kMaxTemp = kTempTelecomsOperationalMax + 10;
    double kMinTemp = 78;
    double dummy_temp_reading = kMinTemp;
    StateId state_before_reading;
    while (dummy_temp_reading < kMaxTemp) {
        state_before_reading = telecoms_temp_state_machine.GetCurrentState();
        telecoms_temp_sensor.SetDummySensorData(dummy_temp_reading);
        telecoms_temp_sensor.TakeReading();

        TEST_ASSERT_EQUAL_INT(
            TelecomsTempStateCheck(dummy_temp_reading, state_before_reading),
            telecoms_temp_state_machine.GetCurrentState());
        dummy_temp_reading += 1;
    }

    while (dummy_temp_reading > kMinTemp) {
        state_before_reading = telecoms_temp_state_machine.GetCurrentState();
        telecoms_temp_sensor.SetDummySensorData(dummy_temp_reading);
        telecoms_temp_sensor.TakeReading();

        TEST_ASSERT_EQUAL_INT(
            TelecomsTempStateCheck(dummy_temp_reading, state_before_reading),
            telecoms_temp_state_machine.GetCurrentState());
        dummy_temp_reading -= 1;
    }
}
