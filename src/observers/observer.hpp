#ifndef SRC_OBSERVERS_OBSERVER_HPP_
#define SRC_OBSERVERS_OBSERVER_HPP_

#include <src/i2c/sensors/i2c_sensor.hpp>

// Forward declaration of I2CSensor.
class I2CSensor;

/**
  Observer class.

  This describes the base observer class from which other observers
  are derived.
*/
class Observer {
public:
  /**
    Virtual function which is called by subjects, i.e. sensors.

    @param sensor The I2C sensor whose reading has changed.
  */
  virtual void update(I2CSensor* sensor) = 0;
};

#endif  // SRC_OBSERVERS_OBSERVER_HPP_
