#include <src/sensors/i2c_sensors/measurables/bms_settings_measurable.h>

BmsSettingsMeasurable::BmsSettingsMeasurable(Bms* bms)
    : I2cMeasurable<BmsSettingsReading>(bms, BmsSettingsReading_init_default) {}

BmsSettingsReading BmsSettingsMeasurable::TakeDirectI2cReading() {
    Bms* bms = static_cast<Bms*>(sensor);
    return bms->GetBmsSettingsReading();
}
