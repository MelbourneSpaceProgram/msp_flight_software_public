#ifndef SRC_SENSORS_I2C_SENSORS_MEASURABLES_I2C_MEASURABLE_H_
#define SRC_SENSORS_I2C_SENSORS_MEASURABLES_I2C_MEASURABLE_H_

#include <src/config/unit_tests.h>
#include <src/sensors/generic_measurable.h>
#include <src/sensors/i2c_sensors/i2c_device.h>
#include <src/sensors/nanopb_measurable.h>
#include <src/util/satellite_time_source.h>
#include <xdc/runtime/Log.h>

template <typename TimestampedNanopbType>
class I2cMeasurable : public NanopbMeasurable<TimestampedNanopbType> {
   public:
    I2cMeasurable(I2cDevice* sensor, TimestampedNanopbType failure_reading)
        : NanopbMeasurable<TimestampedNanopbType>(failure_reading),
          sensor(sensor) {}

    TimestampedNanopbType TakeDirectNanopbReading() {
        if (kI2cAvailable) {
            return TakeDirectI2cReading();
        } else {
            return this->failure_reading;
        }
    }

    virtual TimestampedNanopbType TakeDirectI2cReading() = 0;

    std::string GetInfoString() { return sensor->GetInfoString(); }

   protected:
    I2cDevice* sensor;
};

#endif  // SRC_SENSORS_I2C_SENSORS_MEASURABLES_NANOPB_MEASURABLE_H_
