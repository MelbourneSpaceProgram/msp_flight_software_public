#ifndef SRC_SENSORS_I2C_SENSORS_MEASURABLES_I2C_MEASURABLE_H_
#define SRC_SENSORS_I2C_SENSORS_MEASURABLES_I2C_MEASURABLE_H_

#include <external/etl/exception.h>
#include <src/board/i2c/i2c.h>
#include <src/config/unit_tests.h>
#include <src/sensors/i2c_sensors/i2c_device.h>
#include <src/sensors/measurable.h>
#include <src/sensors/reading.h>
#include <src/util/satellite_time_source.h>
#include <xdc/runtime/Log.h>

template <typename R>
class I2cMeasurable : public Reading<R>, public Measurable {
   public:
    I2cMeasurable(I2cDevice* sensor, R failure_reading)
        : sensor(sensor), failure_reading(failure_reading) {}
    virtual ~I2cMeasurable() {}

    bool TakeReading() {
        // Sensor can be NULL if it is actually not a real sensor (ie a mocked software sensor)
        // Assume mocked software sensors cannot fail.
        if (sensor != NULL && sensor->IsFailed()) {
            this->reading = failure_reading;
            return false;
        }
        try {
            this->reading =
                TakeDirectI2cReading();  // Throws exception on failure
            this->timestamp = SatelliteTimeSource::GetTime();
            this->NotifyObservers();
            return true;
        } catch (etl::exception e) {
            this->reading = failure_reading;
            try {
                this->timestamp = SatelliteTimeSource::GetTime();
            } catch (etl::exception& e) {
                this->timestamp = {0, false};
            }
            if (i2c_available) {
                Log_error2("Failed to read from sensor 0x%02x on bus %c",
                           sensor->GetI2cAddress(),
                           sensor->GetI2cBus()->GetBusLabel());
            }
            return false;
        }
    }
    virtual R TakeDirectI2cReading() = 0;

   protected:
    I2cDevice* sensor;

   private:
    R failure_reading;
};

#endif  // SRC_SENSORS_I2C_SENSORS_MEASURABLES_I2C_MEASURABLE_H_
