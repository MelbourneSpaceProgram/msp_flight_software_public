#ifndef SRC_BOARD_DEBUG_INTERFACE_DEBUG_STREAM_H_
#define SRC_BOARD_DEBUG_INTERFACE_DEBUG_STREAM_H_

#include <src/board/uart/uart.h>
#include <src/util/data_types.h>
#include <ti/sysbios/knl/Semaphore.h>

class DebugStream {
   public:
    static DebugStream *GetInstance();  // Initial call is not thread safe
    bool RequestMessageFromSimulator(byte message_code, byte *response_buffer,
                                     uint8_t response_size);
    void PostMessageToDebugClient(byte message_code, uint8_t payload_size,
                                  byte *payload);

   private:
    Semaphore_Params bus_available_params;
    Semaphore_Handle bus_available;
    Uart debug_uart;
    DebugStream();
    static DebugStream *instance;

    static const uint32_t kTimeoutMillis = 1000;
};

#endif  // SRC_BOARD_DEBUG_INTERFACE_DEBUG_STREAM_H_
