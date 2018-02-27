#include <src/system/sensor_state_machines/battery_temp_state_machine.h>
#include <src/sensors/specific_sensors/test_i2c_sensor.h>

BatteryTempStateMachine::BatteryTempStateMachine(StateManager* state_manager)
    : SensorStateMachine<TestI2cSensor>(state_manager, kBatteryTempNominal) {}

void BatteryTempStateMachine::Update() {
    TestI2cSensor* sensor_with_reading = GetSensorWithReading();
    if (sensor_with_reading != NULL) {
        UpdateState(sensor_with_reading->GetReading());
    }
}

void BatteryTempStateMachine::UpdateState(double temp) {
    switch (GetCurrentState()) {
        case kBatteryTempCriticalLow:
            if (temp > kTempBatteryOperationalLow + kHysteresis) {
                SetStateAndNotify(kBatteryTempNominal);
            }
            break;
        case kBatteryTempNominal:
            if (temp < kTempBatteryOperationalLow) {
                SetStateAndNotify(kBatteryTempCriticalLow);
            } else if (temp > kTempBatteryOperationalHigh + kHysteresis) {
                SetStateAndNotify(kBatteryTempCriticalHigh);
            }
            break;
        case kBatteryTempCriticalHigh:
            if (temp < kTempBatteryOperationalHigh) {
                SetStateAndNotify(kBatteryTempNominal);
            }
            break;
    }
}
