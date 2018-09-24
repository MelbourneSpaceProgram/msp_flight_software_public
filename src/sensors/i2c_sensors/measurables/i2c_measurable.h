#ifndef SRC_SENSORS_I2C_SENSORS_MEASURABLES_I2C_MEASURABLE_H_
#define SRC_SENSORS_I2C_SENSORS_MEASURABLES_I2C_MEASURABLE_H_

#include <external/etl/exception.h>
#include <src/board/i2c/i2c.h>
#include <src/config/unit_tests.h>
#include <src/sensors/generic_measurable.h>
#include <src/sensors/i2c_sensors/i2c_device.h>
#include <src/util/satellite_time_source.h>
#include <xdc/runtime/Log.h>

template <typename TimestampedNanopbType>
class I2cMeasurable : public GenericMeasurable<TimestampedNanopbType> {
    friend class I2cMeasurableManager;

   public:
    I2cMeasurable(I2cDevice* sensor, TimestampedNanopbType failure_reading)
        : sensor(sensor),
          failure_reading(failure_reading),
          first_reading(true) {}
    virtual ~I2cMeasurable() {}

    bool TakeReading() {
        try {
            this->last_reading =
                TakeDirectI2cReading();  // Throws exception on failure

            Time current_time = SatelliteTimeSource::GetTime();
            if (current_time.is_valid) {
                this->last_reading.timestamp_ms = current_time.timestamp_ms;
            } else {
                this->last_reading.timestamp_ms = 0;
            }

            this->NotifyObservers();
            return true;
        } catch (etl::exception e) {
            this->last_reading = failure_reading;
            this->last_reading.timestamp_ms =
                SatelliteTimeSource::GetTime().timestamp_ms;
            if (kI2cAvailable) {
                Log_error3(
                    "Failed to read from sensor 0x%02x on bus %c (mux line "
                    "0x%02x)",
                    sensor->GetI2cAddress(), sensor->GetI2cBus()->GetBusLabel(),
                    sensor->GetMultiplexerChannel());
            }
            return false;
        }
    }

    TimestampedNanopbType GetReading() { return this->last_reading; }
    virtual TimestampedNanopbType TakeDirectI2cReading() = 0;

   protected:
    I2cDevice* sensor;

   private:
    TimestampedNanopbType failure_reading;
    bool first_reading;
};

#endif  // SRC_SENSORS_I2C_SENSORS_MEASURABLES_I2C_MEASURABLE_H_
