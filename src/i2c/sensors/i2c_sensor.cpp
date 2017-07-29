#include <src/i2c/sensors/i2c_sensor.hpp>
#include "iostream"

using namespace std;

/**
  Public methods of the I2CSensor class.
*/

I2CSensor::I2CSensor(I2CBus* bus, int address) : bus(bus), address(address) {}

I2CBus* I2CSensor::get_bus(void) {
  return this->bus;
}

int I2CSensor::get_address(void) {
  return this->address;
}
