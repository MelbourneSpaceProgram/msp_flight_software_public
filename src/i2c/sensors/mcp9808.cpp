#include <src/i2c/sensors/mcp9808.hpp>

const int MCP9808::TEMP_REGISTER = 0x05;

MCP9808::MCP9808(I2CBus* bus, int address) : I2CSensor(bus, address) {}

double MCP9808::get_reading() {
  uint8_t  read_buffer[2];
  uint8_t  write_buffer[1];

  // Tell the MCP9808 to read from the temperature register.
  write_buffer[0] = MCP9808::TEMP_REGISTER;

  // Perform I2C transaction.
  this->get_bus()->perform_transaction(this->get_address(), read_buffer, 2, write_buffer, 1);

  // Perform the necessary bit-shifting on the read buffer.
  double temperature = ((read_buffer[0] & ~(7 << 5)) << 8) | (read_buffer[1]);

  temperature /= 16.0;

  return temperature;
}
