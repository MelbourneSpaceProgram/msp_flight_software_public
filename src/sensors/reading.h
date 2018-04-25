#ifndef SRC_SENSORS_READING_H_
#define SRC_SENSORS_READING_H_

#include <src/util/data_types.h>
#include <src/util/satellite_time_source.h>

template <typename T>
class Reading {
   public:
    T GetReading() const { return reading; }
    Time GetTimestamp() const { return timestamp; }

   protected:
    // TODO(dingbenjamin) Remove default constructor after derived classes
    // updated to have default reading value
    Reading() : timestamp(SatelliteTimeSource::GetTime()) {
        // Invalidate the empty reading
        timestamp.is_valid = false;
    }

    explicit Reading(T reading)
        : timestamp(SatelliteTimeSource::GetTime()), reading(reading) {}

    T reading;
    Time timestamp;
};

#endif  //  SRC_SENSORS_READING_H_
