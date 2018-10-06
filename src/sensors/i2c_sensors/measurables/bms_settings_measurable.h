#ifndef SRC_SENSORS_I2C_SENSORS_MEASURABLES_BMS_SETTINGS_MEASURABLE_H_
#define SRC_SENSORS_I2C_SENSORS_MEASURABLES_BMS_SETTINGS_MEASURABLE_H_

#include <src/board/i2c/bms/bms.h>
#include <src/messages/BmsSettingsReading.pb.h>
#include <src/sensors/i2c_sensors/measurables/i2c_measurable.h>

class BmsSettingsMeasurable : public I2cMeasurable<BmsSettingsReading> {
   public:
    explicit BmsSettingsMeasurable(Bms* bms);
    BmsSettingsReading TakeDirectI2cReading();
};

#endif  // SRC_SENSORS_I2C_SENSORS_MEASURABLES_BMS_SETTINGS_MEASURABLE_H_
