#ifndef SRC_SENSORS_I2C_SENSORS_MEASURABLES_RTIME_MEASURABLE_H_
#define SRC_SENSORS_I2C_SENSORS_MEASURABLES_RTIME_MEASURABLE_H_

#include <src/sensors/i2c_sensors/measurables/i2c_measurable.h>
#include <src/sensors/i2c_sensors/rtc.h>

class RTimeMeasurable : public I2cMeasurable<RTime> {
   public:
    RTimeMeasurable(Rtc* rtc);
    RTime TakeDirectI2cReading();

   private:
    static const RTime kFailedRTimeReading;
};

#endif  // SRC_SENSORS_I2C_SENSORS_MEASURABLES_RTIME_MEASURABLE_H_
