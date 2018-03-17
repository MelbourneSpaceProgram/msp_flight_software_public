#include <src/i2c/multiplexers/i2c_multiplexer.h>

I2cMultiplexer::I2cMultiplexer(I2c* bus, byte address)
    : bus(bus), address(address) {}

I2c* I2cMultiplexer::GetBus() { return bus; }

byte I2cMultiplexer::GetAddress() const { return address; }

void I2cMultiplexer::OpenChannel(uint8_t channel) {
    byte write_buffer = GetChannelStates();
    write_buffer |=  (1 << channel);
    GetBus()->PerformWriteTransaction(GetAddress(), &write_buffer, 1);
}

void I2cMultiplexer::CloseChannel(uint8_t channel) {
    byte write_buffer = GetChannelStates();
    write_buffer &=  ~(1 << channel);
    GetBus()->PerformWriteTransaction(GetAddress(), &write_buffer, 1);
}

void I2cMultiplexer::CloseAllChannels() {
    byte write_buffer = 0;
    GetBus()->PerformWriteTransaction(GetAddress(), &write_buffer, 1);
}

byte I2cMultiplexer::GetChannelStates() {
    byte read_buffer;
    GetBus()->PerformReadTransaction(GetAddress(), &read_buffer, 1);
    return read_buffer;
}
