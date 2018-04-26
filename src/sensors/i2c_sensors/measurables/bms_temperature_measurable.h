#ifndef SRC_SENSORS_I2C_SENSORS_MEASURABLES_BMS_TEMPERATURE_MEASURABLE_H_
#define SRC_SENSORS_I2C_SENSORS_MEASURABLES_BMS_TEMPERATURE_MEASURABLE_H_

#include <src/sensors/i2c_sensors/measurables/i2c_measurable.h>
#include <src/i2c/bms/bms.h>

class BmsTemperatureMeasurable : public I2cMeasurable<double>{
   public:
    explicit BmsTemperatureMeasurable(Bms* temp_sensor);
    double TakeI2cReading();
};

#endif  // SRC_SENSORS_I2C_SENSORS_MEASURABLES_BMS_TEMPERATURE_MEASURABLE_H_
