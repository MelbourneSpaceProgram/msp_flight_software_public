/*
 * DebugStream.h
 *
 *  Created on: 19Nov.,2017
 *      Author: Anthony
 */

#ifndef DEBUGSTREAM_H_
#define DEBUGSTREAM_H_

#include <src/CDH/util/MessageQueue.h>
#include <src/messages/TemperatureMessage.h>

class DebugStream : public MessageHandler<TemperatureMessage>
{
private:
    UARTConfiguration debugConfig;
    UART debug;
public:
    DebugStream();
    virtual ~DebugStream();
    void handleMessage( TemperatureMessage *message );
};

#endif /* DEBUGSTREAM_H_ */
