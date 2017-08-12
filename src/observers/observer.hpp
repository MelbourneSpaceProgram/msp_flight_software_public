/**
  observer.hpp
  Purpose: This is the abstract observer class.

  @author Brett Clark
  @version 1.0
  @date 12/8/2017
*/

#ifndef _OBSERVER_HPP_
#define _OBSERVER_HPP_

#include <src/i2c/sensors/i2c_sensor.hpp>

using namespace std;

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

#endif // _OBSERVER_HPP_
