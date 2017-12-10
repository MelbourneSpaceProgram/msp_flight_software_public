/*
 * DebugStream.h
 *
 *  Created on: 19Nov.,2017
 *      Author: Anthony
 */

#ifndef DEBUGSTREAM_H_
#define DEBUGSTREAM_H_

#include <src/messages/TemperatureMessage.h>
#include <stdint.h>

class DebugStream {
private:
    UARTConfiguration debug_config;
    UART debug;
public:
    DebugStream();
    virtual ~DebugStream();
    void SendMessage(SerialisedMessage);
    uint8_t ReceiveCode();
};

#endif /* DEBUGSTREAM_H_ */
