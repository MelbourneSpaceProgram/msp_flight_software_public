#ifndef SRC_SENSORS_I2C_SENSORS_MEASURABLES_TEMPERATURE_MEASURABLE_H_
#define SRC_SENSORS_I2C_SENSORS_MEASURABLES_TEMPERATURE_MEASURABLE_H_

#include <src/sensors/i2c_sensors/measurables/i2c_measurable.h>
#include <src/sensors/i2c_sensors/mcp9808.hpp>

class TemperatureMeasurable : public I2cMeasurable<double>{
   public:
    TemperatureMeasurable(MCP9808* temp_sensor);
    double TakeI2cReading();
};

#endif  // SRC_SENSORS_I2C_SENSORS_MEASURABLES_TEMPERATURE_MEASURABLE_H_
