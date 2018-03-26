#ifndef SRC_SENSORS_I2C_SENSORS_I2C_SENSOR_H_
#define SRC_SENSORS_I2C_SENSORS_I2C_SENSOR_H_

#include <src/i2c/i2c.h>
#include <src/observers/observer.h>
#include <src/sensors/generic_sensor.h>
#include <string>

template <typename T>
class I2cSensor : public GenericSensor<T> {
   public:
    I2cSensor(const I2c* bus, int address) : bus(bus), address(address) {}
    I2c* GetBus() { return bus; }
    byte GetAddress() { return address; }

   protected:
    const I2c* bus;
    byte address;
};

#endif  //  SRC_SENSORS_I2C_SENSORS_I2C_SENSOR_H_
