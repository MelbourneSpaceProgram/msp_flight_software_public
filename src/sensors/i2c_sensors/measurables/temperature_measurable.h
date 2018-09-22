#ifndef SRC_SENSORS_I2C_SENSORS_MEASURABLES_TEMPERATURE_MEASURABLE_H_
#define SRC_SENSORS_I2C_SENSORS_MEASURABLES_TEMPERATURE_MEASURABLE_H_

#include <src/messages/TemperatureReading.pb.h>
#include <src/sensors/i2c_sensors/mcp9808.h>
#include <src/sensors/i2c_sensors/measurables/i2c_measurable.h>

class TemperatureMeasurable : public I2cMeasurable<TemperatureReading> {
   public:
    TemperatureMeasurable(Mcp9808* temp_sensor);
    TemperatureReading TakeDirectI2cReading();
};

#endif  // SRC_SENSORS_I2C_SENSORS_MEASURABLES_TEMPERATURE_MEASURABLE_H_
