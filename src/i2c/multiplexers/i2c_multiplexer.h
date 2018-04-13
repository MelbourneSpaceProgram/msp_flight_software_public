#ifndef SRC_I2C_MULTIPLEXERS_I2C_MULTIPLEXER_H_
#define SRC_I2C_MULTIPLEXERS_I2C_MULTIPLEXER_H_

#include <src/i2c/i2c.h>

class I2cMultiplexer {
   public:
    enum MuxChannel {
        kMuxChannel0 = 0x00,
        kMuxChannel1 = 0x01,
        kMuxChannel2 = 0x02,
        kMuxChannel3 = 0x04,
        kMuxChannel4 = 0x08,
        kMuxChannel5 = 0x10,
        kMuxChannel6 = 0x20,
        kMuxChannel7 = 0x40,
        kMuxNoChannel = 0xFF
    };

    I2cMultiplexer(const I2c* bus, byte address);
    const I2c* GetBus() const;
    byte GetAddress() const;
    void OpenChannel(MuxChannel channel) const;
    void CloseChannel(MuxChannel channel) const;
    void CloseAllChannels() const;
    byte GetChannelStates() const;

   private:
    const I2c* bus;
    const byte address;
};

#endif  // SRC_I2C_MULTIPLEXERS_I2C_MULTIPLEXER_H_
