#ifndef SRC_SENSORS_I2C_SENSORS_MEASURABLES_BMS_DIE_TEMPERATURE_MEASURABLE_H_
#define SRC_SENSORS_I2C_SENSORS_MEASURABLES_BMS_DIE_TEMPERATURE_MEASURABLE_H_

#include <src/board/i2c/bms/bms.h>
#include <src/sensors/i2c_sensors/measurables/i2c_measurable.h>

class BmsDieTemperatureMeasurable : public I2cMeasurable<double> {
   public:
    explicit BmsDieTemperatureMeasurable(Bms* temp_sensor);
    double TakeDirectI2cReading();

   private:
    static const double kFailedBmsDieTemperatureMeasurable = -9999;
};

#endif  // SRC_SENSORS_I2C_SENSORS_MEASURABLES_BMS_DIE_TEMPERATURE_MEASURABLE_H_
