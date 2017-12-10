/**
  i2c_configuration.hpp
  Purpose: The UART configuration that is used on the board.

  @author Brett Clark
  @version 1.0
  @date 3/5/2017
*/

#ifndef _UART_CONFIGURATION_HPP_
#define _UART_CONFIGURATION_HPP_

/**
  UARTConfiguration class.

  This describes the UART configuration.
*/
class UARTConfiguration {

public:

  /**
    This defines the possible transfer modes for the UART.
  */
  enum transfer_modes {
    BLOCKING_MODE
  };

  /**
    This defines the possible bit rates for the UART.
  */
  enum bit_rates {
    BAUD_115200 = 115200,
    BAUD_9600 = 9600
  };

  /**
    UARTConfiguration constructor.

    @return The UARTConfiguration object.
  */
  // TODO Can we change the signature of transfer_mode to be an enum?
  // Same with bit_rate
  UARTConfiguration(enum transfer_modes transfer_mode, enum bit_rates bit_rate);

  UARTConfiguration();

  /**
    Function that returns the transfer mode.

    @return The UART transfer mode.
  */
  int get_transfer_mode(void);

  /**
    Function that returns the bit rate.

    @return The UART bit rate.
  */
  int get_bit_rate(void);


private:

  /**
    The transfer mode used in the UART.
    Option are listed in transfer_modes enum.
  */
  int transfer_mode;

  /**
    The bit rate used in the UART.
    Option are listed in bit_rates enum.
  */
  int bit_rate;

};

#endif // _UART_CONFIGURATION_HPP_
