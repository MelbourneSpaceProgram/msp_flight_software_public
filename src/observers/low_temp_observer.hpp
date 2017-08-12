/**
  low_temp_observer.hpp
  Purpose: This is a template for writing an observer.

  @author Brett Clark
  @version 1.0
  @date 12/8/2017
*/

#ifndef _LOW_TEMP_OBSERVER_HPP_
#define _LOW_TEMP_OBSERVER_HPP_

#include <src/observers/observer.hpp>

/**
  LowTempObserver class.

  This describes a simple observer that turns on an LED when
  the temperature becomes too low.
*/
class LowTempObserver : public Observer {

public:
  /**
    This function is called by the subject, i.e. the sensor, when a
    reading has been taken.

    @param sensor The sensor for which the reading was taken.
  */
  void update(I2CSensor* sensor);
};

#endif // _LOW_TEMP_OBSERVER_HPP_
