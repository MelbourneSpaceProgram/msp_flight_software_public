#ifndef SRC_UART_UART_CONFIGURATION_H_
#define SRC_UART_UART_CONFIGURATION_H_

#include <src/util/data_types.h>
#include <ti/drivers/UART.h>

/**
  UARTConfiguration class.

  This describes the UART configuration.
*/
class UartConfiguration {
   public:
    static const uint32_t kBaud115200 = 115200;
    static const uint32_t kBaud9600 = 9600;

    /**
      UARTConfiguration default constructor.

      @return The UARTConfiguration object.
    */

    UartConfiguration();

    /**
      UARTConfiguration constructor.

      @param bit_rate The bit rate of the UART
      @return The UARTConfiguration object.
    */

    explicit UartConfiguration(uint32_t bit_rate);

    /**
      UARTConfiguration constructor that takes a UART_Params arg

      @param uart_params The parameters for the UARTConfiguration
      @return The UARTConfiguration object.
    */

    explicit UartConfiguration(UART_Params uart_params);

    /**
      Returns the UART Parameters

      @return The UART Parameters
     */

    const UART_Params& GetUARTParams() const;

   private:
    /**
      The parameters for the UARTConfiguration
     */
    UART_Params uart_params;
};

#endif  // SRC_UART_UART_CONFIGURATION_H_
