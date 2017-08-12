#include <src/i2c/i2c_bus.hpp>

using namespace std;

I2CBus::I2CBus(I2CConfiguration* config, int index) : config(config), index(index) {}

void I2CBus::open() {
  int transfer_mode = this->config->get_transfer_mode();
  int bit_rate = this->config->get_bit_rate();

  // Create the I2C_Params object for compatibility.
  I2C_Params i2c_params = {
    transferMode: static_cast<I2C_TransferMode>(transfer_mode),
    bitRate: static_cast<I2C_BitRate>(bit_rate)
  };

  // Set I2C_Params defaults.
  I2C_Params_init(&i2c_params);

  // Get a handle to the I2C bus.
  this->handle = I2C_open(this->index, &i2c_params);
}

void I2CBus::close() {
    I2C_close(this->handle);
}

I2C_Handle I2CBus::get_handle() {
  return this->handle;
}

bool I2CBus::perform_transaction(int address, uint8_t* read_buffer, int read_buffer_length, uint8_t* write_buffer, int write_buffer_length) {
  I2C_Transaction i2c_transaction;
  bool success;

  // Write the buffers and lengths.
  i2c_transaction.slaveAddress = address;
  i2c_transaction.writeBuf = write_buffer;
  i2c_transaction.writeCount = write_buffer_length;
  i2c_transaction.readBuf = read_buffer;
  i2c_transaction.readCount = read_buffer_length;

  I2C_Handle handle = this->get_handle();

  // Perform I2C transfer.
  success = I2C_transfer(handle, &i2c_transaction);

  // Handle the I2C error.
  if (!success) {
  }

  return success;
}

bool I2CBus::perform_write_transaction(int address, uint8_t* write_buffer, int write_buffer_length) {
  return perform_transaction(address, NULL, 0, write_buffer, write_buffer_length);
}

bool I2CBus::perform_read_transaction(int address, uint8_t* read_buffer, int read_buffer_length) {
  return perform_transaction(address, read_buffer, read_buffer_length, NULL, 0);
}
