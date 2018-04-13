#include <src/i2c/multiplexers/i2c_multiplexer.h>
#include <xdc/runtime/Log.h>

I2cMultiplexer::I2cMultiplexer(const I2c* bus, byte address)
    : bus(bus), address(address) {}

const I2c* I2cMultiplexer::GetBus() const { return bus; }

byte I2cMultiplexer::GetAddress() const { return address; }

void I2cMultiplexer::OpenChannel(MuxChannel channel) const {
    if (channel == kMuxNoChannel) {
        Log_error0("Attempted to open invalid mux channel");
    } else {
        byte write_buffer = GetChannelStates();
        write_buffer |= (1 << static_cast<uint8_t>(channel));
        GetBus()->PerformWriteTransaction(GetAddress(), &write_buffer, 1);
    }
}

void I2cMultiplexer::CloseChannel(MuxChannel channel) const {
    if (channel == kMuxNoChannel) {
        Log_error0("Attempted to close invalid mux channel");
    } else {
        byte write_buffer = GetChannelStates();
        write_buffer &= ~(1 << static_cast<uint8_t>(channel));
        GetBus()->PerformWriteTransaction(GetAddress(), &write_buffer, 1);
    }
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
