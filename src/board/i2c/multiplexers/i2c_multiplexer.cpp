#include <src/board/i2c/i2c.h>
#include <src/board/i2c/multiplexers/i2c_multiplexer.h>
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

        bool success = GetBus()->PerformWriteTransaction(GetAddress(), &write_buffer, 1);

        if (!success){
            Log_error1("Mux failed to open channel for channel %d", channel);
        }

        byte channel_states = GetChannelStates();

        if (write_buffer != channel_states){
            Log_error3("Mux responded but channel not open (race condition?): got %d, expected %d", channel, channel_states, write_buffer);
        }
    }
}

I2cMultiplexer::MuxChannel I2cMultiplexer::ChannelIndexToMuxChannel(
    uint8_t channel_index) {
    I2cMultiplexer::MuxChannel channel;
    switch (channel_index) {
        case 0:
            channel = I2cMultiplexer::kMuxChannel0;
            break;
        case 1:
            channel = I2cMultiplexer::kMuxChannel1;
            break;
        case 2:
            channel = I2cMultiplexer::kMuxChannel2;
            break;
        case 3:
            channel = I2cMultiplexer::kMuxChannel3;
            break;
        case 4:
            channel = I2cMultiplexer::kMuxChannel4;
            break;
        case 5:
            channel = I2cMultiplexer::kMuxChannel5;
            break;
        case 6:
            channel = I2cMultiplexer::kMuxChannel6;
            break;
        case 7:
            channel = I2cMultiplexer::kMuxChannel7;
            break;
        default:
            Log_error0("Attempted to open invalid mux channel");
            return I2cMultiplexer::kMuxNoChannel;
    }

    return channel;
}

void I2cMultiplexer::OpenChannel(uint8_t channel_index) const {
    MuxChannel channel = ChannelIndexToMuxChannel(channel_index);
    this->OpenChannel(channel);
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

void I2cMultiplexer::CloseChannel(uint8_t channel_index) const {
    MuxChannel channel = ChannelIndexToMuxChannel(channel_index);
    this->CloseChannel(channel);
}

void I2cMultiplexer::CloseAllChannels() const {
    byte write_buffer = 0;
    GetBus()->PerformWriteTransaction(GetAddress(), &write_buffer, 1);
}

byte I2cMultiplexer::GetChannelStates() const {
    byte read_buffer = 0;
    bool success = GetBus()->PerformReadTransaction(GetAddress(), &read_buffer, 1);

    if(!success){
        Log_error0("Unable to get channel states for mux");
    }
    return read_buffer;
}
