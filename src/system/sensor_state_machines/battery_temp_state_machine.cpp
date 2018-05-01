#include <src/sensors/i2c_sensors/measurables/bms_temperature_measurable.h>
#include <src/system/sensor_state_machines/battery_temp_state_machine.h>

BatteryTempStateMachine::BatteryTempStateMachine(StateManager* state_manager)
    : SensorStateMachine<BmsTemperatureMeasurable>(state_manager,
                                                   kBatteryTempNominal) {}

void BatteryTempStateMachine::Update() {
    BmsTemperatureMeasurable* sensor_with_reading = GetSensorWithReading();
    if (sensor_with_reading != NULL) {
        UpdateState(sensor_with_reading->GetReading());
    }
}

void BatteryTempStateMachine::UpdateState(double temp) {
    switch (GetCurrentState()) {
        case kBatteryTempNominal:
            if (temp > kTempBatteryOperationalHigh) {
                SetStateAndNotify(kBatteryTempCriticalHigh);
            }
        case kBatteryTempCriticalHigh:
            if (temp < kTempBatteryOperationalHigh - kHysteresis) {
                SetStateAndNotify(kBatteryTempNominal);
            }
            break;
    }
}
