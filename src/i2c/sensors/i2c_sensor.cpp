#include <src/i2c/sensors/i2c_sensor.hpp>

using namespace std;

I2CSensor::I2CSensor(I2CBus* bus, int address) : bus(bus), address(address) {}

I2CBus* I2CSensor::get_bus(void) {
  return bus;
}

int I2CSensor::get_address(void) {
  return address;
}

double I2CSensor::get_reading(void) {
  return reading;
}

void I2CSensor::add_observer(Observer* observer) {
  observers.push_back(observer);
}

void I2CSensor::notify_observers(void) {
  for (int i = 0; i < observers.size(); i++) {
      observers.at(i)->update(this);
  }
}
