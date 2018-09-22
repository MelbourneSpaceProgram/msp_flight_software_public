#ifndef SRC_SENSORS_GENERIC_MEASURABLE_H_
#define SRC_SENSORS_GENERIC_MEASURABLE_H_

#include <src/sensors/measurable.h>

template <typename T>
class GenericMeasurable : public Measurable {
   public:
    virtual T GetReading() = 0;

   protected:
    T last_reading;
};

#endif  //  SRC_SENSORS_GENERIC_MEASURABLE_H_
