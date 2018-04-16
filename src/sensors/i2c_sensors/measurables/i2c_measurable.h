#ifndef SRC_SENSORS_I2C_SENSORS_MEASURABLES_I2C_MEASURABLE_H_
#define SRC_SENSORS_I2C_SENSORS_MEASURABLES_I2C_MEASURABLE_H_

#include <external/etl/exception.h>
#include <src/sensors/i2c_sensors/i2c_sensor.h>
#include <src/sensors/measurable.h>
#include <src/sensors/reading.h>

template <typename R>
class I2cMeasurable : public Reading<R>, public Measurable {
   public:
    I2cMeasurable(I2cSensor* sensor) : sensor(sensor) {}
    virtual ~I2cMeasurable() {}

    bool TakeReading() {
        try {
            if (sensor != NULL) sensor->MuxSelect();
            this->reading = TakeI2cReading();  // Throws exception on failure
            if (sensor != NULL) sensor->MuxDeselect();
            // TODO(dingbenjamin): Get time from timekeeper
            RTime current_time = {0, 0, 0, 0, 0, 0};
            this->timestamp = current_time;
            this->NotifyObservers();
            return true;
        } catch (etl::exception e) {
            // TODO(dingbenjamin): Log sensor failure and remedy
            if (sensor != NULL) sensor->MuxDeselect();
            return false;
        }
    }
    virtual R TakeI2cReading() = 0;

    RTime GetTimestamp() const { return Reading<R>::timestamp; }
    void SetTimestamp(RTime timestamp) { Reading<R>::timestamp = timestamp; }

   protected:
    I2cSensor* sensor;
};

#endif  // SRC_SENSORS_I2C_SENSORS_MEASURABLES_I2C_MEASURABLE_H_
