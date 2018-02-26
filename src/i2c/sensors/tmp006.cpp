#include <src/i2c/sensors/tmp006.hpp>
#include "cmath"
#include "iostream"

/**
  Constants of the TMP006 class.
*/

const int TMP006::SENSOR_VOLTAGE_REGISTER = 0x00;
const int TMP006::DIE_TEMP_REGISTER = 0x01;

/**
  Public methods of the TMP006 class.
*/

TMP006::TMP006(I2c* bus, int address, std::string id, double constant)
                : I2CSensor(bus, address, id), constant(constant) {}

void TMP006::TakeReading(void) {
  // double sensor_voltage = get_sensor_voltage();
  double die_temperature = get_die_temperature();

  // double object_temp = get_object_temperature(sensor_voltage,
  // die_temperature);

  // return object_temp;
  reading = die_temperature;
}

/**
  Private methods of the TMP006 class.
*/

double TMP006::get_sensor_voltage(void) {
  return 3.5;
}

double TMP006::get_die_temperature(void) {
  uint8_t  read_buffer[2];
  uint8_t  write_buffer[1];

  // Tell the TMP006 to read from the die temperature register.
  write_buffer[0] = TMP006::DIE_TEMP_REGISTER;

  // Perform I2C transaction.
  bus->PerformTransaction(this->get_address(), read_buffer, 2, write_buffer, 1);

  // Perform necessary bitshifting on read buffer.
  double temperature = (read_buffer[0] << 6) | (read_buffer[1] >> 2);

  temperature /= 32.0;

  return temperature;
}



double TMP006::get_object_temperature(double sensor_voltage,
                                      double die_temperature) {
  double object_temp_power_4 = std::pow(die_temperature, 4) + (sensor_voltage /
                                                          this->constant);

  // Handle negative reading.
  if (object_temp_power_4 < 0) {
    return NULL;
  }

  return std::pow(object_temp_power_4, 1.0 / 4);
}
