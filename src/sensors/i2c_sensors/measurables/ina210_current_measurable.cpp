#include <src/sensors/i2c_sensors/measurables/ina210_current_measurable.h>

Ina210CurrentMeasurable::Ina210CurrentMeasurable(Ina210* current_sensor)
    : I2cMeasurable<double>(current_sensor) {}

double Ina210CurrentMeasurable::TakeI2cReading() {
    Ina210* current_sensor = static_cast<Ina210*>(I2cMeasurable::sensor);
    return current_sensor->TakeCurrentReading();
}
