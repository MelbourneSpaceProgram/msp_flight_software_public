#ifndef SRC_SENSOR_I2C_SENSORS_MEASURABLES_BMS_TEMPERATURE_MEASURABLE_H_
#define SRC_SENSOR_I2C_SENSORS_MEASURABLES_BMS_TEMPERATURE_MEASURABLE_H_

#include <src/board/i2c/bms/bms.h>
#include <src/messages/BmsTemperatureReading.pb.h>
#include <src/sensors/i2c_sensors/measurables/i2c_measurable.h>

class BmsTemperatureMeasurable : public I2cMeasurable<BmsTemperatureReading> {
public:
    explicit BmsTemperatureMeasurable(Bms* bms);
    BmsTemperatureReading TakeDirectI2cReading();
};
#endif  // SRC_SENSOR_2C_SENSORS_MEASURABLES_BMS_TEMPERATURE_MEASURABLE_H_
