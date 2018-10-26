#include <src/sensors/measurable_manager.h>
#include <src/sensors/test_sensors/test_i2c_measurable.h>
#include <src/system/sensor_state_machines/telecoms_temp_state_machine.h>
#include <src/telecomms/lithium.h>

TelecomsTempStateMachine::TelecomsTempStateMachine(StateManager* state_manager)
    : SensorStateMachine<TemperatureMeasurable>(state_manager,
                                                kTelecomsTempNominal) {}

void TelecomsTempStateMachine::Update() {
    TemperatureMeasurable* sensor_with_reading = GetSensorWithReading();

    if (sensor_with_reading == NULL) {
        Log_error0(
            "Null pointer obtained when updating telecomms temp state "
            "machine.");
        return;
    }

    TelecomsIndex id;
    MeasurableManager* measurable_manager = MeasurableManager::GetInstance();
    TemperatureMeasurable* telecoms_1_sensor =
        dynamic_cast<TemperatureMeasurable*>(
            measurable_manager->GetMeasurable<TemperatureReading>(kComT1));
    TemperatureMeasurable* telecoms_2_sensor =
        dynamic_cast<TemperatureMeasurable*>(
            measurable_manager->GetMeasurable<TemperatureReading>(kComT2));

    if (sensor_with_reading == telecoms_1_sensor) {
        id = kTelecoms1;
    } else if (sensor_with_reading == telecoms_2_sensor) {
        id = kTelecoms2;
    } else {
        Log_error0(
            "Failed to identify telecomms temperature sensor when updating "
            "telecomms temperature state machine.");
        return;
    }

    UpdateState(sensor_with_reading->GetReading().temp, id);
}

void TelecomsTempStateMachine::UpdateState(float temp, TelecomsIndex id) {
    if (id == kTelecoms1) {
        telecoms_1_temp = temp;
    } else if (id == kTelecoms2) {
        telecoms_2_temp = temp;
    }

    switch (GetCurrentState()) {
        case kTelecomsTempCriticalLow:
            if ((telecoms_1_temp > kTempTelecomsOperationalMin) &&
                (telecoms_2_temp > kTempTelecomsOperationalMin)) {
                SetStateAndNotify(kTelecomsTempNominal);
                Log_info0("Telecomms temperature nominal: turning on Lithium");
                Lithium::GetInstance()->UnlockState(
                    Lithium::kCriticalTempCondition);
            }
            break;
        case kTelecomsTempNominal:
            if ((telecoms_1_temp > kTempTelecomsOperationalMax + kHysteresis) ||
                (telecoms_2_temp > kTempTelecomsOperationalMax + kHysteresis)) {
                SetStateAndNotify(kTelecomsTempCriticalHigh);
                Log_info0(
                    "Telecomms temperature critical high: Shutting off "
                    "Lithium");
                Lithium::GetInstance()->LockState(
                    Lithium::kCriticalTempCondition);

            } else if ((telecoms_1_temp <
                        kTempTelecomsOperationalMin - kHysteresis) ||
                       (telecoms_1_temp <
                        kTempTelecomsOperationalMin - kHysteresis)) {
                SetStateAndNotify(kTelecomsTempCriticalLow);
                Log_info0(
                    "Telecomms temperature critical low: Shutting off Lithium");
                Lithium::GetInstance()->LockState(
                    Lithium::kCriticalTempCondition);
            }
            break;
        case kTelecomsTempCriticalHigh:
            if ((telecoms_1_temp < kTempTelecomsOperationalMax) &&
                (telecoms_2_temp < kTempTelecomsOperationalMax)) {
                SetStateAndNotify(kTelecomsTempNominal);
                Log_info0("Telecomms temperature nominal: turning on Lithium");
                Lithium::GetInstance()->UnlockState(
                    Lithium::kCriticalTempCondition);
            }
            break;
    }
}
