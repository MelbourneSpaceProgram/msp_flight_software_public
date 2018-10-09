#include <src/messages/TemperatureReading.pb.h>
#include <src/sensors/measurable_id.h>
#include <src/sensors/measurable_manager.h>
#include <src/system/sensor_state_machines/battery_temp_state_machine.h>

BatteryTempStateMachine::BatteryTempStateMachine(StateManager* state_manager)
    : SensorStateMachine<BmsBatteryTemperatureMeasurable>(
          state_manager, kBatteryTempNominal) {}

void BatteryTempStateMachine::Update() {
    BmsBatteryTemperatureMeasurable* sensor_with_reading =
        GetSensorWithReading();
    if (sensor_with_reading == NULL) {
        etl::exception e(
            "Null pointer obtained when updating battery temp state machine.",
            __FILE__, __LINE__);
        throw e;
    }

    BatteryIndex id;
    MeasurableManager* measurable_manager = MeasurableManager::GetInstance();
    BmsBatteryTemperatureMeasurable* battery_1_sensor =
        dynamic_cast<BmsBatteryTemperatureMeasurable*>(
            measurable_manager->GetMeasurable<TemperatureReading>(
                kEpsBmsBatT1));
    BmsBatteryTemperatureMeasurable* battery_2_sensor =
        dynamic_cast<BmsBatteryTemperatureMeasurable*>(
            measurable_manager->GetMeasurable<TemperatureReading>(
                kEpsBmsBatT2));

    if (sensor_with_reading == battery_1_sensor) {
        id = kBattery1;
    } else if (sensor_with_reading == battery_2_sensor) {
        id = kBattery2;
    } else {
        etl::exception e(
            "Failed to identify battery temperature sensor when updating "
            "battery temperature state machine.",
            __FILE__, __LINE__);
        throw e;
    }

    UpdateState(sensor_with_reading->GetReading().temp, id);
}

void BatteryTempStateMachine::UpdateState(double temp, BatteryIndex id) {
    if (id == kBattery1) {
        battery_1_temp = temp;
    } else if (id == kBattery2) {
        battery_2_temp = temp;
    }

    switch (GetCurrentState()) {
        case kBatteryTempNominal:
            if ((battery_1_temp > kTempBatteryOperationalHigh) ||
                (battery_2_temp > kTempBatteryOperationalHigh)) {
                SetStateAndNotify(kBatteryTempCriticalHigh);
            }
            break;
        case kBatteryTempCriticalHigh:
            if ((battery_1_temp < kTempBatteryOperationalHigh - kHysteresis) &&
                (battery_2_temp < kTempBatteryOperationalHigh - kHysteresis)) {
                SetStateAndNotify(kBatteryTempNominal);
            }
            break;
    }
}
