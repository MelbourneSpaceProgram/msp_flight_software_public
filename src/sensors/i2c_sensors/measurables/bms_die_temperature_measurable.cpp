#include <src/sensors/i2c_sensors/measurables/bms_die_temperature_measurable.h>

BmsDieTemperatureMeasurable::BmsDieTemperatureMeasurable(Bms* temp_sensor)
    : I2cMeasurable<double>(temp_sensor, kFailedBmsDieTemperatureMeasurable) {}

double BmsDieTemperatureMeasurable::TakeDirectI2cReading() {
    Bms* temp_sensor = static_cast<Bms*>(I2cMeasurable::sensor);
    return temp_sensor->TakeI2cDieTempReading();
}
