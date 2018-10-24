#ifndef SRC_SENSORS_MEASURABLE_H_
#define SRC_SENSORS_MEASURABLE_H_

#include <external/etl/vector.h>

class Observer;

class Measurable {
   public:
    virtual ~Measurable(){};
    virtual bool TakeReading() = 0;
    bool AddObserver(Observer* observer);
    bool HasNewReading();

   protected:
    Measurable();
    void NotifyObservers();

   private:
    static constexpr uint8_t kMaxObservers = 3;
    etl::vector<Observer*, kMaxObservers> observers;
    bool new_reading;
};

#endif  //  SRC_SENSORS_MEASURABLE_H_
