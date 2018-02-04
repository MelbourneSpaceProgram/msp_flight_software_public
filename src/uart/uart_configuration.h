#ifndef SRC_UART_UART_CONFIGURATION_H_
#define SRC_UART_UART_CONFIGURATION_H_

#include <src/util/data_types.h>
#include <ti/drivers/UART.h>

class UartConfiguration {
   public:
    static const uint32_t kBaud115200 = 115200;
    static const uint32_t kBaud9600 = 9600;
    UartConfiguration();
    explicit UartConfiguration(uint32_t bit_rate);
    explicit UartConfiguration(UART_Params uart_params);
    const UART_Params& GetUARTParams() const;

   private:
    UART_Params uart_params;
};

#endif  // SRC_UART_UART_CONFIGURATION_H_
