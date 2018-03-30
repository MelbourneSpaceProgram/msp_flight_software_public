#ifndef SRC_SENSORS_GENERIC_MEASURABLE_H_
#define SRC_SENSORS_GENERIC_MEASURABLE_H_

#include <src/sensors/measurable.h>
#include <src/sensors/reading.h>

template <typename T>
class GenericMeasurable : public Measurable, public Reading<T> {};

#endif  //  SRC_SENSORS_GENERIC_MEASURABLE_H_
