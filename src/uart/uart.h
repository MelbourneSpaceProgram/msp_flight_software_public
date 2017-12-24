#ifndef SRC_UART_UART_H_
#define SRC_UART_UART_H_

#include <src/uart/uart_configuration.h>
#include <src/util/data_types.h>
#include <ti/drivers/UART.h>

/**
  UARTBus class.

  This describes the UART configuration.
*/
class Uart {
   public:
    /**
      UARTBus constructor.

      @param config The UARTConfiguration used by the bus.
      @param index The index of the UART bus on the board.
      @return The UARTBus object.
    */
    Uart(UartConfiguration config, uint8_t index);

    /**
      UART destructor, closes the UART bus
     */
    ~Uart();

    /**
      Function that returns a handle to the open UART bus. Must be called after
      the UART_Bus::open method.

      @return The UART_Handle.
    */
    UART_Handle GetHandle() const;

    /**
      Returns a constant reference to the configuration of the UART bus

      @return The UART Configuration
    */
    const UartConfiguration& GetConfig() const;

    /**
      Function that performs an UART write operation.

      @param address The address of the UART device on the bus.
      @param write_buffer The buffer which will be used to perform the initial
      write to the UART device.
      @param write_buffer_length The length of the write buffer.
    */
    bool PerformWriteTransaction(const byte* write_buffer,
                                 uint16_t write_buffer_length) const;

    /**
      Function that performs an UART write operation.

      @param read_buffer The buffer to which the UART device will write.
      @param read_buffer_length The length of the read buffer.
    */
    bool PerformReadTransaction(byte* read_buffer,
                                uint16_t read_buffer_length) const;

   private:
    /**
      Private copy constructor to ensure no double deletion
    */
    Uart(const Uart&);

    /**
      Private assignment operator to ensure no double deletion
    */
    Uart& operator=(const Uart&);

    /**
      Function that opens the UART bus.
    */
    void Open();

    /**
      Function that closes the UART bus.
    */
    void Close();

    /**
      The UART configuration for the bus.
    */
    UartConfiguration config;

    /**
      The index of the UART bus on the board.
    */
    const uint8_t index;

    /**
      The handle to the open UART bus.
    */
    UART_Handle handle;
};

#endif  // SRC_UART_UART_H_
