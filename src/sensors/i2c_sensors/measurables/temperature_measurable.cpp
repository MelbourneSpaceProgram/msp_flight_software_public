#include <external/etl/exception.h>
#include <src/sensors/i2c_sensors/measurables/temperature_measurable.h>
#include <src/util/data_types.h>

TemperatureMeasurable::TemperatureMeasurable(Mcp9808* temp_sensor)
    : I2cMeasurable<TemperatureReading>(temp_sensor,
                                        TemperatureReading_init_default) {}

TemperatureReading TemperatureMeasurable::TakeDirectI2cReading() {
    Mcp9808* temp_sensor = static_cast<Mcp9808*>(I2cMeasurable::sensor);
    TemperatureReading reading = TemperatureReading_init_default;
    reading.temp = temp_sensor->TakeI2cReading();
    return reading;
}
