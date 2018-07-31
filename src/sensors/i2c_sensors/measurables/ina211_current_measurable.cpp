#include <src/sensors/i2c_sensors/measurables/ina211_current_measurable.h>

Ina211CurrentMeasurable::Ina211CurrentMeasurable(Ina211* current_sensor)
    : I2cMeasurable<double>(current_sensor, kFailedCurrentReading) {}

double Ina211CurrentMeasurable::TakeDirectI2cReading() {
    Ina211* current_sensor = static_cast<Ina211*>(I2cMeasurable::sensor);
    return current_sensor->TakeCurrentReading();
}
