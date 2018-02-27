#ifndef SRC_OBSERVERS_SPECIFIC_OBSERVERS_TEST_OBSERVER_H_
#define SRC_OBSERVERS_SPECIFIC_OBSERVERS_TEST_OBSERVER_H_

#include <src/observers/generic_observer.h>

class TestIntSensor;

class TestObserver : public GenericObserver<TestIntSensor> {
   public:
    void Update();
    int GetObservedReading() const;

   private:
    int observed_reading;
};

#endif  //  SRC_OBSERVERS_SPECIFIC_OBSERVERS_TEST_OBSERVER_H_
