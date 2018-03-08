#ifndef SRC_SENSORS_SENSOR_H_
#define SRC_SENSORS_SENSOR_H_

#include <external/etl/vector.h>

class Observer;

class Sensor {
   public:
    virtual bool TakeReading() = 0;
    bool AddObserver(Observer* observer);
    bool HasNewReading();

   protected:
    Sensor();
    void NotifyObservers();

   private:
    static const uint8_t kMaxObservers = 3;
    etl::vector<Observer*, kMaxObservers> observers;
    bool new_reading;
};

#endif  //  SRC_SENSORS_SENSOR_H_
