#include "src/messages/serialised_message.h"
#include <stddef.h>
#include <stdint.h>
#include "src/util/data_types.h"

SerialisedMessage::SerialisedMessage(uint16_t size) : size(size) {
    buffer = new byte[size];
}

SerialisedMessage::SerialisedMessage(uint16_t size, byte* message)
    : size(size) {
    buffer = new byte[size];
    std::memcpy(buffer, message, (size_t)size);
}

SerialisedMessage::~SerialisedMessage() {
    if (buffer != NULL) {
        delete[] buffer;
        buffer = NULL;
    }
}
