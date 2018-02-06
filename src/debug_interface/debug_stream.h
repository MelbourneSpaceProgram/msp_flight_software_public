#ifndef SRC_DEBUG_INTERFACE_DEBUG_STREAM_H_
#define SRC_DEBUG_INTERFACE_DEBUG_STREAM_H_

#include <src/messages/temperature_message.h>
#include <src/tasks/task_holder.h>
#include <src/uart/uart.h>
#include <ti/sysbios/knl/Queue.h>

typedef struct QueueMessage {
    Queue_Elem element;
    Queue_Elem *element_pointer;
    SerialisedMessage *serial_message;
} QueueMessage;

class DebugStream {
    friend class RunnableDebugStream;

   public:
    static DebugStream *GetInstance();  // Initial call is not thread safe
    void EnqueueSerialised(Message *message);
    // TODO(dingbenjamin): Find a way to make this private and still call unit
    // tests, currently anyone who uses this is at high risk of memory leak
    SerialisedMessage *DequeueSerialised();

   private:
    DebugStream();
    uint8_t ReceiveCode() const;
    void SendMessage(const SerialisedMessage &serial_msg) const;

    static DebugStream *instance;
    Uart debug_uart;
    Queue_Handle debug_message_queue;
};

#endif  // SRC_DEBUG_INTERFACE_DEBUG_STREAM_H_
