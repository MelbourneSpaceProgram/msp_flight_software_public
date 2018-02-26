#ifndef SRC_SENSORS_SENSOR_H_
#define SRC_SENSORS_SENSOR_H_

#include <external/etl/vector.h>

class Observer;

class Sensor {
   public:
    virtual void TakeReading() = 0;
    bool AddObserver(Observer* observer);
    bool HasNewReading();

   protected:
    Sensor();
    void NotifyObservers();
    bool new_reading;

   private:
    static const uint8_t kMaxObservers = 3;
    etl::vector<Observer*, kMaxObservers> observers;
};

#endif  //  SRC_SENSORS_SENSOR_H_
