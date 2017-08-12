#ifndef _OBSERVER_H_
#define _OBSERVER_H_

#include <src/i2c/sensors/i2c_sensor.hpp>

using namespace std;

// Forward declaration of I2CSensor.
class I2CSensor;

class Observer {
public:
  virtual void update(I2CSensor* sensor) = 0;
};

#endif // _OBSERVER_H_
