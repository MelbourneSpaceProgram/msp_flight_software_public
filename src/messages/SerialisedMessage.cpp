/*
 * SerialisedMessage.cpp
 *
 *  Created on: 24Nov.,2017
 *      Author: Anthony
 */

#include <src/messages/SerialisedMessage.h>

SerialisedMessage::SerialisedMessage(int messageSize) {
    buffer = new char[messageSize];
    size = messageSize;
}

SerialisedMessage::SerialisedMessage(int messageSize, char* message) {
    buffer = new char[messageSize];
    size = messageSize;
    std::memcpy(&buffer, &message, size);
}

SerialisedMessage::~SerialisedMessage() {
    // TODO Auto-generated destructor stub
}
