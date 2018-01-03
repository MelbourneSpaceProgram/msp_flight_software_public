#ifndef SRC_CDH_UTIL_DEBUG_STREAM_H_
#define SRC_CDH_UTIL_DEBUG_STREAM_H_

#include <src/messages/temperature_message.h>
#include <src/tasks/task_holder.h>
#include <src/uart/uart.h>
#include <src/uart/uart_configuration.h>

// TODO(dingbenjamin): Consider making singleton? Sort of a Singleton as only
// InitDebugStream should be able to create a DebugStream object - can't think
// of any use cases outside of that. May also need to consider splitting the
// creation of the DebugStream into a different class which also holds the debug
// message queue
class DebugStream {
    friend class RunnableDebugStream;

   private:
    DebugStream();
    Uart debug_uart;

   public:
    void SendMessage(const SerialisedMessage &serial_msg);
    uint8_t ReceiveCode();
};

#endif  // SRC_CDH_UTIL_DEBUG_STREAM_H_
