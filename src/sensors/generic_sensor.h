#ifndef SRC_SENSORS_GENERIC_SENSOR_H_
#define SRC_SENSORS_GENERIC_SENSOR_H_

#include <src/sensors/sensor.h>
#include <src/sensors/reading.h>

template<typename T>
class GenericSensor : public Sensor, public Reading<T> {
};

#endif  //  SRC_SENSORS_GENERIC_SENSOR_H_
