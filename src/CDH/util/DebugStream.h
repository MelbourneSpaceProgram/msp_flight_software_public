/*
 * DebugStream.h
 *
 *  Created on: 19Nov.,2017
 *      Author: Anthony, Ben
 */

#ifndef DEBUGSTREAM_H_
#define DEBUGSTREAM_H_

#include <src/messages/TemperatureMessage.h>
#include <src/uart/uart_configuration.hpp>
#include <src/uart/uart.hpp>
#include <stdint.h>

class DebugStream {
private:
    static void SendTestMessage(DebugStream debug_stream, char data);
    UARTConfiguration debug_config;
    UART debug;
public:
    static void *InitDebugStream();
    DebugStream();
    virtual ~DebugStream();
    void SendMessage(SerialisedMessage serial_msg);
    uint8_t ReceiveCode();
};

#endif /* DEBUGSTREAM_H_ */
