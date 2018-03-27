#include <src/i2c/multiplexers/i2c_multiplexer.h>

I2cMultiplexer::I2cMultiplexer(const I2c* bus, byte address)
    : bus(bus), address(address) {}

const I2c* I2cMultiplexer::GetBus() const { return bus; }

byte I2cMultiplexer::GetAddress() const { return address; }

void I2cMultiplexer::OpenChannel(uint8_t channel) const {
    byte write_buffer = GetChannelStates();
    write_buffer |= (1 << channel);
    GetBus()->PerformWriteTransaction(GetAddress(), &write_buffer, 1);
}

void I2cMultiplexer::CloseChannel(uint8_t channel) const {
    byte write_buffer = GetChannelStates();
    write_buffer &= ~(1 << channel);
    GetBus()->PerformWriteTransaction(GetAddress(), &write_buffer, 1);
}

void I2cMultiplexer::CloseAllChannels() const {
    byte write_buffer = 0;
    GetBus()->PerformWriteTransaction(GetAddress(), &write_buffer, 1);
}

byte I2cMultiplexer::GetChannelStates() const {
    byte read_buffer;
    GetBus()->PerformReadTransaction(GetAddress(), &read_buffer, 1);
    return read_buffer;
}
