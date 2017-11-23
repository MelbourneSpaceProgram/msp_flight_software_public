#include <ti/drivers/UART.h>
#include <src/uart/uart_configuration.hpp>
#include <src/uart/uart.hpp>

UART::UART(UARTConfiguration* config, int index) : config(config), index(index) {
    this->open();
};

UART::UART() {
};

void UART::open() {
  int transfer_mode = this->config->get_transfer_mode();
  int bit_rate = this->config->get_bit_rate();


  UART_Params uart_params = {
      writeDataMode: UART_DATA_BINARY,
      readDataMode: UART_DATA_BINARY,
      readReturnMode: UART_RETURN_FULL,
      readEcho: UART_ECHO_OFF,
      baudRate: bit_rate,
      readMode: UART_MODE_BLOCKING
  };

  // Set I2C_Params defaults.
  UART_Params_init(&uart_params);

  // Get a handle to the UART bus.
  this->handle = UART_open(this->index, &uart_params);
}

void UART::close() {
    UART_close(this->handle);
}

UART_Handle UART::get_handle() {
  return this->handle;
}

bool UART::perform_write_transaction(char* write_buffer, int write_buffer_length){
    UART_write(this->handle, (void *) write_buffer, write_buffer_length);

    // TODO What is the return value?
}

bool UART::perform_read_transaction(char* read_buffer, int read_buffer_length){

    UART_read(this->handle, (void *) read_buffer, read_buffer_length);

    // TODO What is the return value?
}
