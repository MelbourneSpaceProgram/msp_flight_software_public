/**
  i2c_configuration.h
  Purpose: The I2C configuration that is used on the board.

  @author Brett Clark
  @version 1.0
  @date 3/5/2017
*/

#ifndef _I2C_CONFIGURATION_H_
#define _I2C_CONFIGURATION_H_

/**
  I2CConfiguration class.

  This describes the I2C configuration.
*/
class I2CConfiguration {

/**
  Public members of the I2CConfiguration class.
*/
public:

  /**
     Public constants of the I2CConfiguration class.
  */

  /**
    This defines the possible transfer modes for the I2C.
  */
  enum transfer_modes {
    BLOCKING_MODE
  };

  /**
    This defines the possible bit rates for the I2C.
  */
  enum bit_rates {
    RATE_100_KHZ,
    RATE_400_KHZ
  };

  /**
    I2CConfiguration constructor.

    @return The I2CConfiguration object.
  */
  I2CConfiguration(int transfer_mode, int bit_rate);

  /**
    Function that returns the transfer mode.
  */
  int get_transfer_mode(void);

  /**
    Function that returns the bit rate.
  */
  int get_bit_rate(void);

/**
  Private members of the I2CConfiguration class.
*/
private:
  
  /**
    Member variables of the I2CConfiguration class.
  */

  /**
    The transfer mode used in the I2C. 
    Option are listed in transfer_modes enum.
  */
  int transfer_mode;

  /**
    The bit rate used in the I2C. 
    Option are listed in bit_rates enum.
  */
  int bit_rate;


};

#endif // _I2C_CONFIGURATION_H_
