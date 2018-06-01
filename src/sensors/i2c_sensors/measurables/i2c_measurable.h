#ifndef SRC_SENSORS_I2C_SENSORS_MEASURABLES_I2C_MEASURABLE_H_
#define SRC_SENSORS_I2C_SENSORS_MEASURABLES_I2C_MEASURABLE_H_

#include <src/config/unit_tests.h>
#include <external/etl/exception.h>
#include <src/sensors/i2c_sensors/i2c_sensor.h>
#include <src/sensors/measurable.h>
#include <src/sensors/reading.h>
#include <src/util/satellite_time_source.h>
#include <xdc/runtime/Log.h>

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
            this->timestamp = SatelliteTimeSource::GetTime();
            this->NotifyObservers();
            return true;
        } catch (etl::exception e) {
            if (sensor != NULL) sensor->MuxDeselect();
            // TODO(dingbenjamin): Change log to output bus and address
            if(i2c_enabled){
                Log_error0("Failed to read from sensor");
            }
            return false;
        }
    }
    virtual R TakeI2cReading() = 0;

   protected:
    I2cSensor* sensor;
};

#endif  // SRC_SENSORS_I2C_SENSORS_MEASURABLES_I2C_MEASURABLE_H_
