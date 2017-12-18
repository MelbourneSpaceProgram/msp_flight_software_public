#include "src/messages/serialised_message.h"
#include <stddef.h>
#include <stdint.h>

SerialisedMessage::SerialisedMessage(uint16_t size) {
    buffer = new byte[size];
    this->size = size;
}

SerialisedMessage::SerialisedMessage(uint16_t size, byte* message) {
    this->size = size;
    buffer = new byte[size];
    std::memcpy(buffer, message, (size_t)size);
}

SerialisedMessage::~SerialisedMessage() { delete[] buffer; }
