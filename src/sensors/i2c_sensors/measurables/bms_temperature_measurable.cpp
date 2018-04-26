#include <src/sensors/i2c_sensors/measurables/bms_temperature_measurable.h>

BmsTemperatureMeasurable::BmsTemperatureMeasurable(Bms* temp_sensor)
    : I2cMeasurable<double>(temp_sensor) {}

double BmsTemperatureMeasurable::TakeI2cReading() {
    Bms* temp_sensor = static_cast<Bms*>(I2cMeasurable::sensor);
    return temp_sensor->TakeI2cTempReading();
}
