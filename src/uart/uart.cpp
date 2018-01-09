#include <src/uart/uart.h>
#include <src/uart/uart_configuration.h>
#include <ti/drivers/UART.h>

Uart::Uart(UartConfiguration config, uint8_t index)
    : config(config), index(index) {
    Open();
}

Uart::~Uart() { Close(); }

void Uart::Open() {
    // Get a handle to the UART bus.
    UART_Params uart_params = config.GetUARTParams();
    this->handle = UART_open(this->index, &uart_params);
    if (this->handle == NULL) {
        // TODO(dingbenjamin): Throw Exception after exceptions designed
    }
}

void Uart::Close() {
    if (this->handle != NULL) {
        UART_close(this->handle);
    }
}

UART_Handle Uart::GetHandle() const { return this->handle; }

const UartConfiguration& Uart::GetConfig() const { return config; }

bool Uart::PerformWriteTransaction(const byte* write_buffer,
                                   uint16_t write_buffer_length) const {
    return UART_write(this->handle, reinterpret_cast<const void*>(write_buffer),
                      write_buffer_length) == write_buffer_length;
}

bool Uart::PerformReadTransaction(byte* read_buffer,
                                  uint16_t read_buffer_length) const {
    return UART_read(this->handle, reinterpret_cast<void*>(read_buffer),
                     read_buffer_length) == read_buffer_length;
}
