#ifndef SRC_SENSORS_I2C_SENSORS_MEASURABLES_BMS_BATTERY_TEMPERATURE_MEASURABLE_H_
#define SRC_SENSORS_I2C_SENSORS_MEASURABLES_BMS_BATTERY_TEMPERATURE_MEASURABLE_H_

#include <src/board/i2c/bms/bms.h>
#include <src/sensors/i2c_sensors/measurables/i2c_measurable.h>

class BmsBatteryTemperatureMeasurable : public I2cMeasurable<double> {
   public:
    explicit BmsBatteryTemperatureMeasurable(Bms* temp_sensor);
    double TakeI2cReading();
};

#endif  // SRC_SENSORS_I2C_SENSORS_MEASURABLES_BMS_BATTERY_TEMPERATURE_MEASURABLE_H_
