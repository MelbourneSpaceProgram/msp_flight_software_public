#ifndef SRC_SENSORS_I2C_SENSORS_MEASURABLES_BMS_DIE_TEMPERATURE_MEASURABLE_H_
#define SRC_SENSORS_I2C_SENSORS_MEASURABLES_BMS_DIE_TEMPERATURE_MEASURABLE_H_

#include <src/i2c/bms/bms.h>
#include <src/sensors/i2c_sensors/measurables/i2c_measurable.h>

class BmsDieTemperatureMeasurable : public I2cMeasurable<double> {
   public:
    explicit BmsDieTemperatureMeasurable(Bms* temp_sensor);
    double TakeI2cReading();
};

#endif  // SRC_SENSORS_I2C_SENSORS_MEASURABLES_BMS_DIE_TEMPERATURE_MEASURABLE_H_
