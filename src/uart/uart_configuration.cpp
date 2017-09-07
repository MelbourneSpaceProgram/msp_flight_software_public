#include <ti/drivers/UART.h>
#include <src/uart/uart_configuration.hpp>

using namespace std;

UARTConfiguration::UARTConfiguration(int transfer_mode, int bit_rate, UART_Callback reader_callback) : transfer_mode(transfer_mode), bit_rate(bit_rate), reader_callback(reader_callback) {}

int UARTConfiguration::get_transfer_mode() {
    return this->transfer_mode;
}

int UARTConfiguration::get_bit_rate() {
    return this->bit_rate;
}

UART_Callback UARTConfiguration::get_reader_callback(){
    return this->reader_callback;
}
