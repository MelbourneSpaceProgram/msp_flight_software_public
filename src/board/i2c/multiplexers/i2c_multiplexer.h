#ifndef SRC_BOARD_I2C_MULTIPLEXERS_I2C_MULTIPLEXER_H_
#define SRC_BOARD_I2C_MULTIPLEXERS_I2C_MULTIPLEXER_H_

#include <src/util/data_types.h>
class I2c;

class I2cMultiplexer {
   public:
    enum MuxChannel {
        kMuxChannel0 = 0x00,
        kMuxChannel1 = 0x01,
        kMuxChannel2 = 0x02,
        kMuxChannel3 = 0x03,
        kMuxChannel4 = 0x04,
        kMuxChannel5 = 0x05,
        kMuxChannel6 = 0x06,
        kMuxChannel7 = 0x07,
        kMuxNoChannel = 0xFF
    };

    I2cMultiplexer(const I2c* bus, byte address);
    const I2c* GetBus() const;
    byte GetAddress() const;
    void OpenChannel(MuxChannel channel) const;
    void OpenChannel(uint8_t channel) const;
    void CloseChannel(MuxChannel channel) const;
    void CloseChannel(uint8_t channel) const;
    static MuxChannel ChannelIndexToMuxChannel(uint8_t channel_index);
    void CloseAllChannels() const;
    byte GetChannelStates() const;

   private:
    const I2c* bus;
    const byte address;
};

#endif  // SRC_BOARD_I2C_MULTIPLEXERS_I2C_MULTIPLEXER_H_
