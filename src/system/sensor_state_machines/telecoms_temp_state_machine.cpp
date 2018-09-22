#include <src/sensors/test_sensors/test_i2c_measurable.h>
#include <src/system/sensor_state_machines/telecoms_temp_state_machine.h>

TelecomsTempStateMachine::TelecomsTempStateMachine(StateManager* state_manager)
    : SensorStateMachine<TestI2cMeasurable>(state_manager,
                                            kTelecomsTempNominal) {}

void TelecomsTempStateMachine::Update() {
    TestI2cMeasurable* sensor_with_reading = GetSensorWithReading();
    if (sensor_with_reading != NULL) {
        UpdateState(sensor_with_reading->GetReading().dummy_data);
    }
}

void TelecomsTempStateMachine::UpdateState(float temp) {
    switch (GetCurrentState()) {
        case kTelecomsTempNominal:
            if (temp > kTempTelecomsOperationalMax + kHysteresis) {
                SetStateAndNotify(kTelecomsTempCriticalHigh);
            }
            break;
        case kTelecomsTempCriticalHigh:
            if (temp < kTempTelecomsOperationalMax) {
                SetStateAndNotify(kTelecomsTempNominal);
            }
            break;
    }
}
