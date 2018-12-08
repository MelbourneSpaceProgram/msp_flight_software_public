#include <src/board/i2c/i2c.h>
#include <src/board/i2c/multiplexers/i2c_multiplexer.h>
#include <src/config/satellite.h>
#include <xdc/runtime/Log.h>

I2cMultiplexer::I2cMultiplexer(const I2c* bus, byte address)
    : bus(bus), address(address) {}

const I2c* I2cMultiplexer::GetBus() const { return bus; }

byte I2cMultiplexer::GetAddress() const { return address; }

bool I2cMultiplexer::OpenChannel(MuxChannel channel) const {
    if (channel == kMuxNoChannel) {
        Log_error0("Attempted to open invalid mux channel");
        return false;
    } else {
        byte write_buffer = 0;
        if (!GetChannelStates(write_buffer)) return false;

        write_buffer |= (1 << static_cast<uint8_t>(channel));

        if (!GetBus()->PerformWriteTransaction(GetAddress(), &write_buffer,
                                               1)) {
            Log_error2("Mux failed to open channel %d on bus %d", channel,
                       GetBus()->index);
            return false;
        }

        byte channel_states = 0;
        if (!GetChannelStates(channel_states)) return false;

        if (write_buffer != channel_states) {
            Log_error3(
                "Mux responded but channel not open (race condition?): got %d, "
                "expected %d on bus %d",
                channel, channel_states, GetBus()->index);
            return false;
        }

        return true;
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

bool I2cMultiplexer::OpenChannel(uint8_t channel_index) const {
    MuxChannel channel = ChannelIndexToMuxChannel(channel_index);
    return OpenChannel(channel);
}

bool I2cMultiplexer::CloseChannel(MuxChannel channel) const {
    if (channel == kMuxNoChannel) {
        Log_error1("Attempted to close invalid mux channel on bus %d",
                   GetBus()->index);
        return false;
    } else {
        byte write_buffer = 0;
        if (!GetChannelStates(write_buffer)) return false;
        write_buffer &= ~(1 << static_cast<uint8_t>(channel));
        if (!GetBus()->PerformWriteTransaction(GetAddress(), &write_buffer,
                                               1)) {
            Log_error1("Failed to close mux channel on bus %d",
                       GetBus()->index);
            return false;
        }
        return true;
    }
}

bool I2cMultiplexer::CloseChannel(uint8_t channel_index) const {
    MuxChannel channel = ChannelIndexToMuxChannel(channel_index);
    return CloseChannel(channel);
}

bool I2cMultiplexer::CloseAllChannels() const {
    byte write_buffer = 0;
    if (!GetBus()->PerformWriteTransaction(GetAddress(), &write_buffer, 1)) {
        if (SystemConfiguration::GetInstance()->IsVerboseLogging())
            Log_error1("Failed to close channels for mux on bus %d",
                       GetBus()->index);
        return false;
    }
    return true;
}

bool I2cMultiplexer::GetChannelStates(byte& states) const {
    if (!GetBus()->PerformReadTransaction(GetAddress(), &states, 1)) {
        Log_error1("Unable to get channel states for mux on bus %d",
                   GetBus()->index);
        return false;
    }
    return true;
}
