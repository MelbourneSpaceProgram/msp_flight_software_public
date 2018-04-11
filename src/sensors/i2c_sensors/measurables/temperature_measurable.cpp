#include <src/sensors/i2c_sensors/measurables/temperature_measurable.h>

TemperatureMeasurable::TemperatureMeasurable(MCP9808* temp_sensor)
    : I2cMeasurable<double>(temp_sensor) {}

double TemperatureMeasurable::TakeI2cReading() {
    MCP9808* temp_sensor = static_cast<MCP9808*>(I2cMeasurable::sensor);
    return temp_sensor->TakeI2cReading();
}
