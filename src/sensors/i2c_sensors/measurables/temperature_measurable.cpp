#include <external/etl/exception.h>
#include <src/sensors/i2c_sensors/measurables/temperature_measurable.h>
#include <src/util/data_types.h>

TemperatureMeasurable::TemperatureMeasurable(Mcp9808* temp_sensor)
    : I2cMeasurable<double>(temp_sensor, kFailedTemperatureReading) {}

double TemperatureMeasurable::TakeDirectI2cReading() {
    Mcp9808* temp_sensor = static_cast<Mcp9808*>(I2cMeasurable::sensor);
    try {
        return temp_sensor->TakeI2cReading();
    } catch (etl::exception& e) {
        return kInvalidDouble;
    }
}
