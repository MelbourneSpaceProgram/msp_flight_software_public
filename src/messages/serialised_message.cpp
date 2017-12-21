#include "src/messages/serialised_message.h"
#include <stddef.h>
#include <stdint.h>

SerialisedMessage::SerialisedMessage(uint16_t size) : size(size) {}

SerialisedMessage::SerialisedMessage(uint16_t size, byte* message)
    : size(size) {
    std::memcpy(buffer, message, (size_t)size);
}
