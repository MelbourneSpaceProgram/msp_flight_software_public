#include <src/sensors/i2c_sensors/measurables/ina212_current_measurable.h>

Ina212CurrentMeasurable::Ina212CurrentMeasurable(Ina212* current_sensor)
    : I2cMeasurable<double>(current_sensor, kFailedCurrentReading) {}

double Ina212CurrentMeasurable::TakeDirectI2cReading() {
    Ina212* current_sensor = static_cast<Ina212*>(I2cMeasurable::sensor);
    return current_sensor->TakeCurrentReading();
}
