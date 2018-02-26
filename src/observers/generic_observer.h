#ifndef SRC_OBSERVERS_GENERIC_OBSERVER_H_
#define SRC_OBSERVERS_GENERIC_OBSERVER_H_

#include <src/observers/observer.h>
#include <src/observers/sensor_container.h>

template <typename T>
class GenericObserver : public Observer, public SensorContainer<T, Observer*> {
   public:
    void RegisterWithSensor(T* sensor) { AddSensor(sensor, this); }
};

#endif  //  SRC_OBSERVERS_GENERIC_OBSERVER_H_
