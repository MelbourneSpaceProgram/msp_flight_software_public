#include <Board.h>
#include <src/CDH/util/debug_stream.h>
#include <src/messages/serialised_message_builder.h>
#include <src/messages/test_message.h>
#include <src/util/data_types.h>
#include <src/util/message_codes.h>

#define UARTA0 MSP_EXP432P401R_UARTA0
#define UARTA2 MSP_EXP432P401R_UARTA2

DebugStream::DebugStream()
    : debug_uart(UartConfiguration(UartConfiguration::kBaud115200), UARTA0) {
    char echo_prompt[] = "Debug stream started.\r\n";
    GPIO_init();
    UART_init();
    debug_uart.PerformWriteTransaction(reinterpret_cast<byte *>(echo_prompt),
                                       sizeof(echo_prompt));
}

void DebugStream::SendMessage(const SerialisedMessage &serial_msg) {
    debug_uart.PerformWriteTransaction(serial_msg.GetBuffer(),
                                       serial_msg.GetSize());
}

uint8_t DebugStream::ReceiveCode() {
    uint8_t read_code[1];
    uint8_t read_code_length = 1;
    debug_uart.PerformReadTransaction(read_code, read_code_length);
    return (uint8_t)read_code[0];
}
