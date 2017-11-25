/*
 * SerialisedMessage.cpp
 *
 *  Created on: 24Nov.,2017
 *      Author: Anthony
 */

#include <src/messages/SerialisedMessage.h>

SerialisedMessage::SerialisedMessage(int messageSize)
{
    buffer = new char[messageSize];
    size = messageSize;
}

SerialisedMessage::~SerialisedMessage()
{
    // TODO Auto-generated destructor stub
}

