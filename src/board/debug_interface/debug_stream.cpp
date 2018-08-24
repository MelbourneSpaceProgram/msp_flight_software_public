#include <src/board/board.h>
#include <src/board/debug_interface/debug_stream.h>
#include <src/messages/serialised_message_builder.h>
#include <src/messages/test_message.h>
#include <src/util/message_codes.h>
#include <src/util/task_utils.h>
#include <ti/sysbios/BIOS.h>
#include <ti/sysbios/knl/Semaphore.h>

DebugStream *DebugStream::instance = NULL;

DebugStream::DebugStream() : debug_uart(UMBILICAL_SIM) {
//DebugStream::DebugStream() : debug_uart(UMBILICAL_CONSOLE) {
    debug_uart.SetBaudRate(Uart::kBaud115200)
        ->SetReadTimeout(TaskUtils::MilliToCycles(kTimeoutMillis))
        ->SetWriteTimeout(TaskUtils::MilliToCycles(kTimeoutMillis))
        ->Open();

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

bool DebugStream::RequestMessageFromSimulator(byte message_code,
                                              byte *response_buffer,
                                              uint8_t response_size) {
    uint8_t message_header[2];
    message_header[0] = message_code;
    message_header[1] = 0x00;

    // Entering critical section
    Semaphore_pend(bus_available, BIOS_WAIT_FOREVER);
    debug_uart.PerformWriteTransaction(message_header, 2);
    int bytes_read =
        debug_uart.PerformReadTransaction(response_buffer, response_size);
    Semaphore_post(bus_available);
    // Exited critical section

    // timeout causes less bytes to be read
    return bytes_read == response_size;
}

void DebugStream::PostMessageToDebugClient(byte message_code,
                                           uint8_t payload_size,
                                           byte *payload) {
    uint8_t message_header[2];
    message_header[0] = message_code;
    message_header[1] = payload_size;

    // Entering critical section
    Semaphore_pend(bus_available, BIOS_WAIT_FOREVER);
    debug_uart.PerformWriteTransaction(message_header, 2);
    debug_uart.PerformWriteTransaction(payload, payload_size);
    Semaphore_post(bus_available);
    // Exited critical section
}
