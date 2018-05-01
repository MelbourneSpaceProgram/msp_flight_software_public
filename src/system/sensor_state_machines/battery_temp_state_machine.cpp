#include <src/system/sensor_state_machines/battery_temp_state_machine.h>
#include <src/sensors/i2c_sensors/measurables/bms_temperature_measurable.h>

BatteryTempStateMachine::BatteryTempStateMachine(StateManager* state_manager)
    : SensorStateMachine<BmsTemperatureMeasurable>(state_manager, kBatteryTempNominal) {}

void BatteryTempStateMachine::Update() {
    BmsTemperatureMeasurable* sensor_with_reading = GetSensorWithReading();
    if (sensor_with_reading != NULL) {
        UpdateState(sensor_with_reading->TakeI2cReading());
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
