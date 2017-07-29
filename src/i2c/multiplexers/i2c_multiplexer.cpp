#include "i2c_multiplexer.h"

I2CMultiplexer::I2CMultiplexer(I2CBus* bus, int address) : bus(bus), address(address) {}

I2CBus* I2CMultiplexer::get_bus(void) {
  return this->bus;
}

int I2CMultiplexer::get_address(void) {
  return this->address;
}
