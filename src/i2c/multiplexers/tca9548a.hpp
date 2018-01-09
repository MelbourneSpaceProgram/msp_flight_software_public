#ifndef SRC_I2C_MULTIPLEXERS_TCA9548A_HPP_
#define SRC_I2C_MULTIPLEXERS_TCA9548A_HPP_

#include <src/i2c/i2c_bus.hpp>
#include <src/i2c/multiplexers/i2c_multiplexer.hpp>

/**
  TCA9548A class.

  This describes a single TCA9548A multiplexer.
*/
class TCA9548A : public I2CMultiplexer {
   public:
    /**
      TCA9548A constructor.

      @param bus The bus that the multiplexer is connected to.
      @param address The address of the multiplexer.
      @return The TCA9548A object.
    */
    TCA9548A(I2CBus* bus, int address);

    /**
      Method that opens a channel of the multiplexer.

      @param channel The channel to open.
    */
    void open_channel(int channel);

    /**
      Function that closes all channels of the multiplexer.
    */
    void close_all_channels(void);

    /**
     Function that returns the states of the channels on the multiplexer.
    */
    uint8_t* get_channel_states(uint8_t* read_buffer);

    /**
      The channels of the TCA9548A multiplexer.
    */
    static const int CHANNEL_0;
    static const int CHANNEL_1;
    static const int CHANNEL_2;
    static const int CHANNEL_3;
    static const int CHANNEL_4;
    static const int CHANNEL_5;
    static const int CHANNEL_6;
    static const int CHANNEL_7;

   private:
    /**
      A reference to the bus that the multiplexer is connected to.
    */
    I2CBus* bus;

    /**
      The address of the multiplexer.
    */
    int address;
};

#endif  // SRC_I2C_MULTIPLEXERS_TCA9548A_HPP_
