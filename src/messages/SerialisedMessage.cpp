/*
 * SerialisedMessage.cpp
 *
 *  Created on: 24Nov.,2017
 *      Author: Anthony, Ben
 */

#include <src/messages/SerialisedMessage.h>
#include <stdint.h>
#include <stddef.h>

SerialisedMessage::SerialisedMessage(uint16_t size) {
    buffer = new char[size];
    this->size = size;
}

SerialisedMessage::SerialisedMessage(uint16_t size, char* message) {
    this->size = size;
    buffer = new char[size];
    std::memcpy(buffer, message, (size_t) size);
}

SerialisedMessage::~SerialisedMessage() {
    // Auto-generated destructor stub
}
