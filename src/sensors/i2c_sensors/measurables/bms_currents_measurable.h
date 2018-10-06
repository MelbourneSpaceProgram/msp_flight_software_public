#ifndef SRC_SENSORS_I2C_SENSORS_MEASURABLES_BMS_CURRENTS_MEASURABLE_H_
#define SRC_SENSORS_I2C_SENSORS_MEASURABLES_BMS_CURRENTS_MEASURABLE_H_

#include <src/board/i2c/bms/bms.h>
#include <src/messages/BmsCurrentsReading.pb.h>
#include <src/sensors/i2c_sensors/measurables/i2c_measurable.h>

class BmsCurrentsMeasurable : public I2cMeasurable<BmsCurrentsReading> {
   public:
    explicit BmsCurrentsMeasurable(Bms* bms);
    BmsCurrentsReading TakeDirectI2cReading();
};

#endif  // SRC_SENSORS_I2C_SENSORS_MEASURABLES_BMS_CURRENTS_MEASURABLE_H_
