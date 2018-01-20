#include <src/i2c/i2c.h>
#include <src/i2c/i2c_configuration.h>
#include <ti/drivers/I2C.h>

I2c::I2c(I2cConfiguration config, uint8_t index)
    : config(config), index(index) {
    Open();
}

I2c::~I2c() { Close(); }

void I2c::Open() {
    I2C_Params I2C_params = config.GetI2cParams();
    handle = I2C_open(index, &I2C_params);
    if (handle == NULL) {
        // TODO(wschuetz) Throw Exception
    }
}

void I2c::Close() {
    if (handle != NULL) {
        I2C_close(handle);
    }
}

I2C_Handle I2c::GetHandle() const { return handle; }

const I2cConfiguration& I2c::GetConfig() const { return config; }

bool I2c::PerformTransaction(byte address, byte* read_buffer,
                         uint16_t read_buffer_length, byte* write_buffer,
                         uint16_t write_buffer_length) const {
    I2C_Transaction i2c_transaction;

    // Write the buffers and lengths.
    i2c_transaction.slaveAddress = address;
    i2c_transaction.writeBuf = write_buffer;
    i2c_transaction.writeCount = write_buffer_length;
    i2c_transaction.readBuf = read_buffer;
    i2c_transaction.readCount = read_buffer_length;

    // Perform I2C transfer.
    bool success = I2C_transfer(handle, &i2c_transaction);

    // Handle the I2C error.
    if (!success) {
        // TODO(wschuetz) Throw Exception
    }

    return success;
}

bool I2c::PerformWriteTransaction(byte address, byte* write_buffer,
                                   uint16_t write_buffer_length) const {
    return I2c::PerformTransaction(address, NULL, 0, write_buffer,
                                   write_buffer_length);
}

bool I2c::PerformReadTransaction(byte address, byte* read_buffer,
                                  uint16_t read_buffer_length) const {
    return I2c::PerformTransaction(address, read_buffer, read_buffer_length,
                                   NULL, 0);
}
