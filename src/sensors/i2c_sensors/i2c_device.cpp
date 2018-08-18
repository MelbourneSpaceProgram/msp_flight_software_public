#include <src/sensors/i2c_sensors/i2c_device.h>

I2cDevice::I2cDevice(const I2c* bus, uint8_t address,
                     const I2cMultiplexer* multiplexer,
                     I2cMultiplexer::MuxChannel channel)
    : bus(bus), address(address), multiplexer(multiplexer), channel(channel) {}

void I2cDevice::MuxSelect() const {
    if (multiplexer != NULL) {
        multiplexer->OpenChannel(channel);
    }
}

void I2cDevice::MuxDeselect() const {
    if (multiplexer != NULL) {
        multiplexer->CloseChannel(channel);
    }
}

const I2c* I2cDevice::GetI2cBus() const { return bus; }

uint8_t I2cDevice::GetI2cAddress() const { return address; }

const I2cMultiplexer* I2cDevice::GetI2cMultiplexer() const {
    return multiplexer;
}

I2cMultiplexer::MuxChannel I2cDevice::GetMultiplexerChannel() const {
    return channel;
}

bool I2cDevice::PerformWriteTransaction(byte address, byte* write_buffer,
                                        uint16_t write_buffer_length) const {
    bool success;
    MuxSelect();
    success = bus->PerformWriteTransaction(address, write_buffer,
                                           write_buffer_length);
    MuxDeselect();
    return success;
}

bool I2cDevice::PerformReadTransaction(byte address, byte* write_buffer,
                                       uint16_t write_buffer_length) const {
    bool success;
    MuxSelect();
    success =
        bus->PerformReadTransaction(address, write_buffer, write_buffer_length);
    MuxDeselect();
    return success;
}

bool I2cDevice::PerformTransaction(byte address, byte* read_buffer,
                                   uint16_t read_buffer_length,
                                   byte* write_buffer,
                                   uint16_t write_buffer_length) const {
    bool success;
    MuxSelect();
    success = bus->PerformTransaction(address, read_buffer, read_buffer_length,
                                      write_buffer, write_buffer_length);
    MuxDeselect();
    return success;
}
