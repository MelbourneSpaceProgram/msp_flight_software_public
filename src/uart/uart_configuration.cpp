#include <ti/drivers/UART.h>
#include <src/uart/uart_configuration.hpp>

using namespace std;

UARTConfiguration::UARTConfiguration(enum transfer_modes transfer_mode, enum bit_rates bit_rate) : transfer_mode(transfer_mode), bit_rate(bit_rate) {}
UARTConfiguration::UARTConfiguration() {};
int UARTConfiguration::get_transfer_mode() {
    return this->transfer_mode;
}

int UARTConfiguration::get_bit_rate() {
    return this->bit_rate;
}


