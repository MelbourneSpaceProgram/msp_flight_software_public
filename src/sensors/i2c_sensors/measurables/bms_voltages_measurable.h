#ifndef SRC_SENSORS_I2C_SENSORS_MEASURABLES_BMS_VOLTAGES_MEASURABLE_H_
#define SRC_SENSORS_I2C_SENSORS_MEASURABLES_BMS_VOLTAGES_MEASURABLE_H_

#include <src/board/i2c/bms/bms.h>
#include <src/messages/BmsVoltagesReading.pb.h>
#include <src/sensors/i2c_sensors/measurables/i2c_measurable.h>

class BmsVoltagesMeasurable : public I2cMeasurable<BmsVoltagesReading> {
   public:
    explicit BmsVoltagesMeasurable(Bms* bms);
    BmsVoltagesReading TakeDirectI2cReading();
};

#endif  // SRC_SENSORS_I2C_SENSORS_MEASURABLES_BMS_VOLTAGES_MEASURABLE_H_
