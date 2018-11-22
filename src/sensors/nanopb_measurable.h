#ifndef SRC_SENSORS_I2C_SENSORS_MEASURABLES_NANOPB_MEASURABLE_H_
#define SRC_SENSORS_I2C_SENSORS_MEASURABLES_NANOPB_MEASURABLE_H_

#include <src/config/satellite.h>
#include <src/sensors/generic_measurable.h>
#include <src/util/msp_exception.h>
#include <src/util/satellite_time_source.h>
#include <xdc/runtime/Log.h>
#include <string>

template <typename TimestampedNanopbType>
class NanopbMeasurable : public GenericMeasurable<TimestampedNanopbType> {
    friend class MeasurableManager;

   public:
    NanopbMeasurable(TimestampedNanopbType failure_reading)
        : failure_reading(failure_reading), first_reading(true) {}
    virtual ~NanopbMeasurable() {}

    bool TakeReading() {
        try {
            this->last_reading =
                TakeDirectNanopbReading();  // Throws exception on failure

            Time current_time = SatelliteTimeSource::GetTime();
            if (current_time.is_valid) {
                this->last_reading.timestamp_ms = current_time.timestamp_ms;
            } else {
                this->last_reading.timestamp_ms = 0;
            }

            return true;
        } catch (MspException& e) {
            this->last_reading = failure_reading;
            this->last_reading.timestamp_ms =
                SatelliteTimeSource::GetTime().timestamp_ms;
            // TODO (rskew) stop lots of logs when kI2cAvailable == false
            MspException::LogException(e, kFailedMeasurableReadingCatch);
            Log_error1("Failed to read from %s",
                       (xdc_IArg)(GetInfoString().c_str()));
            return false;
        }
    }

    TimestampedNanopbType GetReading() { return this->last_reading; }
    TimestampedNanopbType GetFailureReading() { return this->failure_reading; }
    virtual TimestampedNanopbType TakeDirectNanopbReading() = 0;
    virtual std::string GetInfoString() = 0;

   private:
    TimestampedNanopbType failure_reading;
    bool first_reading;
};

#endif  // SRC_SENSORS_I2C_SENSORS_MEASURABLES_NANOPB_MEASURABLE_H_
