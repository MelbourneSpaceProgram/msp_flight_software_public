#ifndef SRC_DEBUG_INTERFACE_DEBUG_STREAM_H_
#define SRC_DEBUG_INTERFACE_DEBUG_STREAM_H_

#include <src/tasks/task_holder.h>
#include <src/uart/uart.h>
#include <src/util/data_types.h>
#include <ti/sysbios/knl/Semaphore.h>

class DebugStream {
   public:
    static DebugStream *GetInstance();  // Initial call is not thread safe
    Uart debug_uart;
    Semaphore_Handle bus_available;
    void RequestMessageFromSimulator(byte message_code);
    void ReceiveMessageFromSimulator(byte *buffer, uint8_t message_size);
    void PostMessageToDebugClient(byte message_code, uint8_t payload_size,
                                  byte *payload);

   private:
    Semaphore_Params bus_available_params;
    DebugStream();
    static DebugStream *instance;
};

#endif  // SRC_DEBUG_INTERFACE_DEBUG_STREAM_H_
