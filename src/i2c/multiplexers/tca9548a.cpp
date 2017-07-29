#include <src/i2c/multiplexers/tca9548a.hpp>

TCA9548A::TCA9548A(I2CBus* bus, int address) : I2CMultiplexer(bus, address) {}

const int TCA9548A::CHANNEL_0 = 0x01;
const int TCA9548A::CHANNEL_1 = 0x02;
const int TCA9548A::CHANNEL_2 = 0x04;
const int TCA9548A::CHANNEL_3 = 0x06;
const int TCA9548A::CHANNEL_4 = 0x10;
const int TCA9548A::CHANNEL_5 = 0x20;
const int TCA9548A::CHANNEL_6 = 0x40;
const int TCA9548A::CHANNEL_7 = 0x80;

void TCA9548A::open_channel(int channel) {
  // Open the channel using I2C transaction.
  uint8_t  write_buffer[1];

  // Tell the TCA9548A which channel to open.
  write_buffer[0] = channel;

  // Perform I2C transaction.
  this->get_bus()->perform_write_transaction(this->get_address(), write_buffer, 1);
}

void TCA9548A::close_all_channels() {
    uint8_t write_buffer[1];

    write_buffer[0] = 0x00;
    // Perform I2C transaction.
      this->get_bus()->perform_write_transaction(this->get_address(), write_buffer, 1);
}

uint8_t* TCA9548A::get_channel_states() {
    uint8_t read_buffer[1];

    this->get_bus()->perform_read_transaction(this->get_address(), read_buffer, 1);

    return read_buffer;
}
