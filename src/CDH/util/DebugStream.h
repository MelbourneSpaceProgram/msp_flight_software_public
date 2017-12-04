/*
 * DebugStream.h
 *
 *  Created on: 19Nov.,2017
 *      Author: Anthony
 */

#ifndef DEBUGSTREAM_H_
#define DEBUGSTREAM_H_

#include <src/messages/TemperatureMessage.h>

class DebugStream {
private:
    UARTConfiguration debugConfig;
    UART debug;
public:
    DebugStream();
    virtual ~DebugStream();
    void sendMessage(SerialisedMessage);
};

#endif /* DEBUGSTREAM_H_ */
