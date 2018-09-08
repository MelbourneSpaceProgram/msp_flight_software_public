#ifndef SRC_BOARD_UART_UART_H_
#define SRC_BOARD_UART_UART_H_

#include <src/util/data_types.h>
#include <ti/drivers/UART.h>

// TODO(dingbenjamin): Write hardware unit test
class Uart {
   public:
    explicit Uart(uint8_t bus_index);
    ~Uart();
    void Open();

    // Must be called after the UART_Bus::open method.
    UART_Handle GetHandle() const;
    int PerformWriteTransaction(const byte* write_buffer,
                                uint16_t write_buffer_length) const;
    int PerformReadTransaction(byte* read_buffer,
                               uint16_t read_buffer_length) const;
    Uart* SetBaudRate(uint32_t baud_rate);
    Uart* SetReadMode(UART_Mode read_mode);
    Uart* SetWriteMode(UART_Mode write_mode);
    Uart* SetReadCallback(UART_Callback read_callback);
    Uart* SetWriteCallback(UART_Callback write_callback);
    Uart* SetReadTimeout(uint32_t read_timeout);
    Uart* SetWriteTimeout(uint32_t write_timeout);

    static const uint32_t kBaud921600 = 921600;
    static const uint32_t kBaud115200 = 115200;
    static const uint32_t kBaud9600 = 9600;

   private:
    // Private copy/assignment to ensure no double deletion
    Uart(const Uart&);
    Uart& operator=(const Uart&);
    void Close();

    UART_Params uart_params;
    const uint8_t bus_index;
    UART_Handle handle;
};

#endif  // SRC_BOARD_UART_UART_H_
