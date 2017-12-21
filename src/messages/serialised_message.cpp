#include "src/messages/serialised_message.h"
#include <stddef.h>
#include <stdint.h>

SerialisedMessage::SerialisedMessage(uint16_t size) {
    if (size < 512 && size != 0) {
        buffer = new byte[size];
        this->size = size;
    } else {
        this->size = 0;
        buffer = NULL;
    }
}

SerialisedMessage::SerialisedMessage(uint16_t size, byte* message) {
    this->size = size;
    buffer = new byte[size];
    if (buffer == NULL) {
        // TODO: Throw exception?
    }
    std::memcpy(buffer, message, (size_t)size);
}

SerialisedMessage::~SerialisedMessage() {
    delete[] buffer;
    buffer = NULL;
}
