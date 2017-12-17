#ifndef DEBUG_STREAM_H_
#define DEBUG_STREAM_H_

#include <stdint.h>
#include "src/messages/temperature_message.h"
#include "src/uart/uart_configuration.hpp"
#include "src/uart/uart.hpp"

// TODO: Consider making singleton? Sort of a Singleton as only InitDebugStream should be able to create
// a DebugStream object - can't think of any use cases outside of that. May also need to consider splitting
// the creation of the DebugStream into a different class which also holds the debug message queue
class DebugStream {
private:
    static void SendTestMessage(DebugStream debug_stream, byte data);
    DebugStream();
    UARTConfiguration debug_config;
    UART debug;
public:
    static void *InitDebugStream();
    virtual ~DebugStream() {};
    void SendMessage(SerialisedMessage serial_msg);
    uint8_t ReceiveCode();
};

#endif /* DEBUG_STREAM_H_ */
