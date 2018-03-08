#include <Board.h>
#include <src/config/board_definitions.h>
#include <src/debug_interface/debug_stream.h>
#include <src/messages/serialised_message_builder.h>
#include <src/messages/test_message.h>
#include <src/util/message_codes.h>

DebugStream *DebugStream::instance = NULL;

DebugStream::DebugStream() : debug_uart(UART_CDH_UMB) {
    debug_uart.SetBaudRate(Uart::kBaud115200)->Open();

    Semaphore_Params_init(&bus_available_params);
    bus_available = Semaphore_create(1, &bus_available_params, NULL);
}

// GetInstance() is not thread safe on first initialisation
// It is recommended that it be called from a single thread only the first time
DebugStream *DebugStream::GetInstance() {
    if (instance == NULL) {
        instance = new DebugStream();
    }
    return instance;
}

void DebugStream::RequestMessageFromSimulator(byte message_code) {
    uint8_t message_header[2];
    message_header[0] = message_code;
    message_header[1] = 0x00;
    debug_uart.PerformWriteTransaction(message_header, 2);
}

void DebugStream::ReceiveMessageFromSimulator(byte *buffer,
                                              uint8_t message_size) {
    debug_uart.PerformReadTransaction(buffer, message_size);
}

void DebugStream::PostMessageToDebugClient(byte message_code,
                                           uint8_t payload_size,
                                           byte *payload) {
    uint8_t message_header[2];
    message_header[0] = message_code;
    message_header[1] = payload_size;
    debug_uart.PerformWriteTransaction(message_header, 2);
    debug_uart.PerformWriteTransaction(payload, payload_size);
}
