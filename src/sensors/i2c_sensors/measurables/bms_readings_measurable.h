#ifndef I2C_SENSORS_MEASURABLES_BMS_READINGS_MEASURABLE_H_
#define I2C_SENSORS_MEASURABLES_BMS_READINGS_MEASURABLE_H_

#include <src/board/i2c/bms/bms.h>
#include <src/messages/BmsReadings.pb.h>
#include <src/sensors/i2c_sensors/measurables/i2c_measurable.h>

class BmsReadingsMeasurable : public I2cMeasurable<BmsReadings> {
   public:
    explicit BmsReadingsMeasurable(Bms* bms);
    BmsReadings TakeDirectI2cReading();
};

#endif  // I2C_SENSORS_MEASURABLES_BMS_READINGS_MEASURABLE_H_
