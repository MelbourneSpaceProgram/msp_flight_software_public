#ifndef _ADS1115_H_
#define _ADS1115_H_

#include "../../i2c/sensors/i2c_sensor.hpp"

class ADS1115 : public I2CSensor {
public:
  ADS1115(I2CBus* bus, int address);
  double get_reading(void);
}

#endif // _ADS1115_H_
