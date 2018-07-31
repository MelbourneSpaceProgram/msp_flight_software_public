#ifndef SRC_SENSORS_I2C_SENSORS_MEASURABLES_INA211_CURRENT_MEASURABLE_H_
#define SRC_SENSORS_I2C_SENSORS_MEASURABLES_INA211_CURRENT_MEASURABLE_H_

#include <src/sensors/i2c_sensors/current_sensors/ina211.h>
#include <src/sensors/i2c_sensors/measurables/i2c_measurable.h>

class Ina211CurrentMeasurable : public I2cMeasurable<double> {
   public:
    explicit Ina211CurrentMeasurable(Ina211* current_sensor);
    double TakeDirectI2cReading();

   private:
    static const double kFailedCurrentReading = -9999;
};

#endif  //  SRC_SENSORS_I2C_SENSORS_MEASURABLES_INA211_CURRENT_MEASURABLE_H_
