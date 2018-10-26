#include <CppUTest/TestHarness.h>
#include <src/config/unit_tests.h>
#include <src/sensors/test_sensors/test_i2c_measurable.h>
#include <src/system/sensor_state_machines/battery_charge_state_machine.h>
#include <src/system/sensor_state_machines/battery_temp_state_machine.h>
#include <src/system/sensor_state_machines/telecoms_temp_state_machine.h>
#include <src/system/state_definitions.h>
#include <src/system/state_manager.h>
#include <string>

static constexpr float kTwoEclipseCapacity = 0.2;
static constexpr float kThreePassCapacity = 0.4;
static constexpr float kHysteresis = 0.02;

static constexpr double kHysteresisTemp = 1;

static constexpr double kTempTelecomsOperationalMax = 80;

TEST_GROUP(SensorStateLogic){};

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

TEST(SensorStateLogic, TestBatteryChargeStateFlow) {
    StateManager* state_manager = StateManager::GetStateManager();
    BatteryChargeStateMachine battery_charge_state_machine(state_manager);

    TestI2cMeasurable battery_soc_sensor;
    battery_charge_state_machine.RegisterWithSensor(&battery_soc_sensor);

    double kMaxSoc = 1;
    double kMinSoc = 0.9;
    double dummy_soc_reading = kMinSoc;
    StateId state_before_reading;
    while (dummy_soc_reading < kMaxSoc) {
        state_before_reading = battery_charge_state_machine.GetCurrentState();
        battery_soc_sensor.SetDummySensorData(dummy_soc_reading);
        battery_soc_sensor.TakeReading();

        CHECK_EQUAL(
            BatteryChargeStateCheck(dummy_soc_reading, state_before_reading),
            battery_charge_state_machine.GetCurrentState());
        dummy_soc_reading += 0.01;
    }

    while (dummy_soc_reading > kMinSoc) {
        state_before_reading = battery_charge_state_machine.GetCurrentState();
        battery_soc_sensor.SetDummySensorData(dummy_soc_reading);
        battery_soc_sensor.TakeReading();

        CHECK_EQUAL(
            BatteryChargeStateCheck(dummy_soc_reading, state_before_reading),
            battery_charge_state_machine.GetCurrentState());
        dummy_soc_reading -= 0.01;
    }
}
