#include <src/i2c/sensors/i2c_sensor.hpp>

I2CSensor::I2CSensor(I2c* bus, int address, std::string id)
    : bus(bus), address(address), id(id) {}

I2c* I2CSensor::get_bus(void) { return bus; }

int I2CSensor::get_address(void) { return address; }

std::string I2CSensor::get_id(void) { return id; }
