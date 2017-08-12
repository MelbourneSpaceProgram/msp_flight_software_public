/**
  i2c_multiplexer.hpp
  Purpose: Abstract class containing shared functionality of all I2C multiplexers.

  @author Brett Clark
  @version 1.0
  @date 3/6/2017
*/

#ifndef _I2CMultiplexer_HPP_
#define _I2CMultiplexer_HPP_

#include <src/i2c/i2c_bus.hpp>

/**
  I2CMultiplexer class.

  This base class describes an I2CMultiplexer.
*/
class I2CMultiplexer {

public:

  /**
    I2CMultiplexer constructor.

    @param bus The bus that the multiplexer is attached to.
    @param address The address of the multiplexer.
    @return The I2CMultiplexer object.
  */
    I2CMultiplexer(I2CBus* bus, int address);

  /**
    Method that returns a reference to the multiplexer bus.

    @return A reference to the multiplexer bus.
  */
  I2CBus *get_bus(void);

  /**
    Method that returns the multiplexer address.

    @return The multiplexer address.
  */
  int get_address(void);

  /**
    Virtual method for opening a channel of the multiplexer.

    @param channel The channel to open.
  */
  virtual void open_channel(int channel) = 0;

  /**
   Virtual method for closing all the channels of the multiplexer.
  */
  virtual void close_all_channels() = 0;

private:

  /**
    The bus that the multiplexer is connected to.
  */
  I2CBus* bus;

  /**
    The address of the multiplexer.
  */
  int address;
};

#endif // _I2CMultiplexer_HPP_
