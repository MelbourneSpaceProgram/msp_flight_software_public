#ifndef SRC_UART_UART_H_
#define SRC_UART_UART_H_

#include <src/uart/uart_configuration.h>
#include <src/util/data_types.h>
#include <ti/drivers/UART.h>

class Uart {
   public:
    Uart(UartConfiguration config, uint8_t bus_index);
    ~Uart();

    // Can only be called after the UART_Bus::open method.
    UART_Handle GetHandle() const;
    const UartConfiguration& GetConfig() const;
    bool PerformWriteTransaction(const byte* write_buffer,
                                 uint16_t write_buffer_length) const;
    bool PerformReadTransaction(byte* read_buffer,
                                uint16_t read_buffer_length) const;

   private:
    // Private copy/assignment to ensure no double deletion
    Uart(const Uart&);
    Uart& operator=(const Uart&);

    void Open();
    void Close();

    UartConfiguration config;
    const uint8_t bus_index;
    UART_Handle handle;
};

#endif  // SRC_UART_UART_H_
