#ifndef SRC_I2C_MULTIPLEXERS_I2C_MULTIPLEXER_H_
#define SRC_I2C_MULTIPLEXERS_I2C_MULTIPLEXER_H_

#include <src/i2c/i2c.h>

class I2cMultiplexer {
   public:
    I2cMultiplexer(I2c* bus, byte address);
    I2c* GetBus();
    byte GetAddress() const;
    void OpenChannel(uint8_t channel);
    void CloseChannel(uint8_t channel);
    void CloseAllChannels();
    byte GetChannelStates();
    enum MuxChannels {
        kMuxChannel0= 0x00,
        kMuxChannel1= 0x01,
        kMuxChannel2= 0x02,
        kMuxChannel3= 0x04,
        kMuxChannel4= 0x08,
        kMuxChannel5= 0x10,
        kMuxChannel6= 0x20,
        kMuxChannel7= 0x40
    };

   private:
    I2c* bus;
    byte address;
};

#endif  // SRC_I2C_MULTIPLEXERS_I2C_MULTIPLEXER_H_
