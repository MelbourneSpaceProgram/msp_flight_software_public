#include <src/sensors/i2c_sensors/measurables/ina169_current_measurable.h>

Ina169CurrentMeasurable::Ina169CurrentMeasurable(Ina169* current_sensor)
    : I2cMeasurable<double>(current_sensor) {}

double Ina169CurrentMeasurable::TakeI2cReading() {
    Ina169* current_sensor = static_cast<Ina169*>(I2cMeasurable::sensor);
    return current_sensor->TakeCurrentReading();
}
