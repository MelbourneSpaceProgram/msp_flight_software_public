/**
  sample_temp_observer.hpp
  Purpose: This is a template for writing an observer.

  @author Brett Clark
  @version 1.0
  @date 12/8/2017
*/

#ifndef _SAMPLE_TEMP_OBSERVER_HPP_
#define _SAMPLE_TEMP_OBSERVER_HPP_

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

#endif // _SAMPLE_TEMP_OBSERVER_HPP_
