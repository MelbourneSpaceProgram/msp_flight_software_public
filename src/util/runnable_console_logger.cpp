#include <assert.h>
#include <src/board/uart/uart.h>
#include <src/sensors/measurable_id.h>
#include <src/sensors/runnable_system_health_check.h>
#include <src/util/runnable_console_logger.h>
#include <src/util/task_utils.h>
#include <stdio.h>
#include <ti/drivers/utils/RingBuf.h>

// These are created outside of the class to make getting C linkage easier
extern "C" {
RingBuf_Object ring_buffer;
byte buffer[5000];
bool wait = false;
}

Uart* RunnableConsoleLogger::debug_uart = NULL;
bool RunnableConsoleLogger::initialised = false;
GateMutexPri_Params RunnableConsoleLogger::console_uart_mutex_params = {NULL};
GateMutexPri_Handle RunnableConsoleLogger::console_uart_mutex = NULL;

RunnableConsoleLogger::RunnableConsoleLogger(Uart* debug_uart) {
    assert(!initialised);
    initialised = true;
    RingBuf_construct(&ring_buffer, buffer, sizeof(buffer));
    RunnableConsoleLogger::debug_uart = debug_uart;

    GateMutexPri_Params_init(&console_uart_mutex_params);
    console_uart_mutex = GateMutexPri_create(&console_uart_mutex_params, NULL);
    if (console_uart_mutex == NULL) {
        throw etl::exception("Failed to create mutex.", __FILE__, __LINE__);
    }
}

fnptr RunnableConsoleLogger::GetRunnablePointer() { return &LogToConsole; }

void RunnableConsoleLogger::LogToConsole() {
    while (1) {
        UartFlush();
        if (wait) {
            TaskUtils::SleepMilli(RunnableConsoleLogger::kWaitTimeMs);
            wait = false;
        }
    }
}

void RunnableConsoleLogger::WriteToDataLogger(uint8_t measurable_id,
                                              byte encoded_message[],
                                              uint8_t message_size) {
    byte packet[5] = {NULL};
    packet[0] = kMeasurableLoggerSyncChar1;
    packet[1] = kMeasurableLoggerSyncChar2;
    packet[2] = message_size;  // Length of packet except header
    packet[3] = measurable_id;
    packet[4] = 0x00;  // TODO(dingbenjamin): Implement checksum

    IArg key = GateMutexPri_enter(console_uart_mutex);
    debug_uart->PerformWriteTransaction(packet, 5);
    debug_uart->PerformWriteTransaction(encoded_message, message_size);
    GateMutexPri_leave(console_uart_mutex, key);
}

void UartPutch(char ch) { RingBuf_put(&ring_buffer, ch); }

void UartFlush() {
    byte encoded_message[RunnableConsoleLogger::kMaxEncodedMessageLength];

    int bytes_available = RingBuf_getCount(&ring_buffer);
    if (bytes_available == -1 || bytes_available == 0) {
        wait = true;
        return;
    }

    uint32_t byte_counter = 0;
    while (byte_counter < RunnableConsoleLogger::kMaxEncodedMessageLength &&
           byte_counter < bytes_available) {
        RingBuf_get(&ring_buffer, encoded_message + byte_counter);
        byte_counter++;
    }

    RunnableConsoleLogger::WriteToDataLogger(kConsoleMessage, encoded_message,
                                             byte_counter);
}
