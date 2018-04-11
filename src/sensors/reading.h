#ifndef SRC_SENSORS_READING_H_
#define SRC_SENSORS_READING_H_

#include <src/util/data_types.h>

template <typename T>
class Reading {
   public:
    T GetReading() const { return reading; }

   protected:
    // TODO(dingbenjamin) Remove default constructor after derived classes
    // updated to have default reading value
    Reading() {}
    explicit Reading(T reading)
        : timestamp({0, 0, 0, 0, 0, 0}), reading(reading) {}

    T reading;
    RTime timestamp;
};

#endif  //  SRC_SENSORS_READING_H_
