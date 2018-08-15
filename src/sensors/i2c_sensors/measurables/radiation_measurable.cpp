#include <src/sensors/i2c_sensors/measurables/radiation_measurable.h>

/**
 * Measurable for radiation reading.
 *
 * Following the structure found in temperature_measurable.cpp and .h, and
 * mcp9808.cpp and .hpp (for the rad_driver implementation).
 *
 * TODO(daniel632): Implement unit tests for the rad_driver and / or
 * radiation_measurable.
 */

RadiationMeasurable::RadiationMeasurable(RadDriver* rad_sensor)
    : I2cMeasurable<double>(rad_sensor, kFailedRadiationReading) {}

double RadiationMeasurable::TakeDirectI2cReading() {
    RadDriver* rad_sensor = static_cast<RadDriver*>(I2cMeasurable::sensor);
    return rad_sensor->TakeI2cReading();
}
