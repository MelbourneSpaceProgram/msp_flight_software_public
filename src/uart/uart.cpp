#include <src/uart/uart.h>
#include <external/etl/exception.h>
#include <ti/drivers/UART.h>

Uart::Uart(uint8_t bus_index) : bus_index(bus_index), handle(NULL) {
    UART_Params_init(&uart_params);
    uart_params.readEcho = UART_ECHO_OFF;
    uart_params.readDataMode = UART_DATA_BINARY;
    uart_params.writeDataMode = UART_DATA_BINARY;
}

Uart::~Uart() { Close(); }

void Uart::Open() {
    // Get a handle to the UART bus.
    this->handle = UART_open(this->bus_index, &uart_params);
    if (this->handle == NULL) {
        etl::exception e("Unable to open UART, possibly in-use", __FILE__,
                         __LINE__);
        throw e;
    }
}

void Uart::Close() {
    if (this->handle != NULL) {
        UART_close(this->handle);
    }
}

UART_Handle Uart::GetHandle() const { return this->handle; }

int Uart::PerformWriteTransaction(const byte* write_buffer,
                                  uint16_t write_buffer_length) const {
    return UART_write(this->handle, reinterpret_cast<const void*>(write_buffer),
                      write_buffer_length);
}

int Uart::PerformReadTransaction(byte* read_buffer,
                                 uint16_t read_buffer_length) const {
    return UART_read(this->handle, reinterpret_cast<void*>(read_buffer),
                     read_buffer_length);
}

Uart* Uart::SetBaudRate(uint32_t bit_rate) {
    uart_params.baudRate = bit_rate;
    return this;
}

Uart* Uart::SetReadMode(UART_Mode read_mode) {
    uart_params.readMode = read_mode;
    return this;
}

Uart* Uart::SetWriteMode(UART_Mode write_mode) {
    uart_params.writeMode = write_mode;
    return this;
}

Uart* Uart::SetReadCallback(UART_Callback read_callback) {
    uart_params.readCallback = read_callback;
    return this;
}

Uart* Uart::SetWriteCallback(UART_Callback write_callback) {
    uart_params.writeCallback = write_callback;
    return this;
}

Uart* Uart::SetReadTimeout(uint32_t read_timeout) {
    uart_params.readTimeout = read_timeout;
    return this;
}

Uart* Uart::SetWriteTimeout(uint32_t write_timeout) {
    uart_params.writeTimeout = write_timeout;
    return this;
}
