#ifndef SRC_SENSORS_I2C_SENSORS_MEASURABLES_INA212_CURRENT_MEASURABLE_H_
#define SRC_SENSORS_I2C_SENSORS_MEASURABLES_INA212_CURRENT_MEASURABLE_H_

#include <src/sensors/i2c_sensors/current_sensors/ina212.h>
#include <src/sensors/i2c_sensors/measurables/i2c_measurable.h>

class Ina212CurrentMeasurable : public I2cMeasurable<double> {
   public:
    explicit Ina212CurrentMeasurable(Ina212* current_sensor);
    double TakeI2cReading();
};

#endif  //  SRC_SENSORS_I2C_SENSORS_MEASURABLES_INA212_CURRENT_MEASURABLE_H_
