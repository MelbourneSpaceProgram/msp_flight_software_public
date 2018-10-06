#ifndef SRC_SENSOR_I2C_SENSORS_MEASURABLES_BMS_OPEARTION_VALUES_MEASURABLE_H_
#define SRC_SENSOR_I2C_SENSORS_MEASURABLES_BMS_OPEARTION_VALUES_MEASURABLE_H_

#include <src/board/i2c/bms/bms.h>
#include <src/messages/BmsSettingsReading.pb.h>
#include <src/sensors/i2c_sensors/measurables/i2c_measurable.h>

class BmsOperationValuesMeasurable
    : public I2cMeasurable<BmsOperationValuesReading> {
   public:
    explicit BmsOperationValuesMeasurable(Bms* bms);
    BmsOperationValuesReading TakeDirectI2cReading();
};

#endif  // SRC_SENSOR_I2C_SENSORS_MEASURABLES_BMS_OPEARTION_VALUES_MEASURABLE_H_
