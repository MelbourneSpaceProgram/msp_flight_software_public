/**
  i2c_bus.hpp
  Purpose: Represents an UART bus.

  @author Brett Clark
  @version 1.0
  @date 3/5/2017
*/

#ifndef _UART_HPP_
#define _UART_HPP_

#include <ti/drivers/UART.h>
#include <src/uart/uart_configuration.hpp>

/**
  UARTBus class.

  This describes the UART configuration.
*/
class UART {

public:

  /**
    UARTBus constructor.

    @param config The UARTConfiguration used by the bus.
    @param index The index of the UART bus on the board.
    @return The UARTBus object.
  */
  UART(UARTConfiguration* config, int index);

  /**
    Function that opens the UART bus.
  */
  void open(void);

  /**
    Function that closes the UART bus.
  */
  void close(void);

  /**
    Function that returns a handle to the open UART bus. Must be called after the UART_Bus::open method.

    @return The UART_Handle.
  */
  UART_Handle get_handle(void);


  /**
    Function that performs an UART write operation.

    @param address The address of the UART device on the bus.
    @param write_buffer The buffer which will be used to perform the initial write to the UART device.
    @param write_buffer_length The length of the write buffer.
  */
  bool perform_write_transaction(char* write_buffer, int write_buffer_length);

  /**
    Function that performs an UART write operation.

    @param address The address of the UART device on the bus.
    @param read_buffer The buffer to which the UART device will write.
    @param read_buffer_length The length of the read buffer.
  */
  bool perform_read_transaction(int address, uint8_t* read_buffer, int read_buffer_length);

private:

  /**
    The UART configuration for the bus.
  */
  UARTConfiguration *config;

  /**
    The index of the UART bus on the board.
  */
  int index;

  /**
    The handle to the open UART bus.
  */
  UART_Handle handle;
};

#endif // _UART_BUS_HPP_
