#include <src/sensors/i2c_sensors/measurables/bms_temperature_measurable.h>

BmsTemperatureMeasurable::BmsTemperatureMeasurable(Bms* bms)
    : I2cMeasurable<BmsTemperatureReading>(
          bms, BmsTemperatureReading_init_default) {}

BmsTemperatureReading BmsTemperatureMeasurable::TakeDirectI2cReading() {
    Bms* bms = static_cast<Bms*>(sensor);
    return bms->GetBmsTemperatureReading();
}
