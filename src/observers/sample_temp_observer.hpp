#ifndef SRC_OBSERVERS_SAMPLE_TEMP_OBSERVER_HPP_
#define SRC_OBSERVERS_SAMPLE_TEMP_OBSERVER_HPP_

#include <src/observers/observer.hpp>

/**
  SampleTempObserver class.

  This describes a simple observer that changes the colour of an LED 
  depending upon the temperature reading.
*/
class SampleTempObserver : public Observer {
public:
  /**
    This function is called by the subject, i.e. the sensor, when a
    reading has been taken.

    @param sensor The sensor for which the reading was taken.
  */
  void update(I2CSensor* sensor);
};

#endif  // SRC_OBSERVERS_SAMPLE_TEMP_OBSERVER_HPP_
