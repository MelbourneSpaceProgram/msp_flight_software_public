#include <src/uart/uart_configuration.h>

UartConfiguration::UartConfiguration() { UART_Params_init(&uart_params); }

UartConfiguration::UartConfiguration(uint32_t bit_rate) {
    UART_Params_init(&uart_params);
    uart_params.baudRate = bit_rate;
}

UartConfiguration::UartConfiguration(UART_Params uart_params)
    : uart_params(uart_params) {}

const UART_Params& UartConfiguration::GetUARTParams() const {
    return uart_params;
}
