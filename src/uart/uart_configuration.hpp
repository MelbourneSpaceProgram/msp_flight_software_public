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
    HIGH = 115200
  };

  /**
    UARTConfiguration constructor.

    @return The UARTConfiguration object.
  */
  UARTConfiguration(int transfer_mode, int bit_rate, UART_Callback reader_callback);

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

  UART_Callback get_reader_callback(void);

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

  UART_Callback reader_callback;

};

#endif // _UART_CONFIGURATION_HPP_
