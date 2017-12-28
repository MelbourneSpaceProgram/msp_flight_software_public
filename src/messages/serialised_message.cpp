#include <src/messages/serialised_message.h>
#include <src/util/data_types.h>
#include <stddef.h>
#include <stdint.h>

SerialisedMessage::SerialisedMessage(uint16_t size, byte* message)
    : size(size), buffer(message) {}

byte* SerialisedMessage::GetBuffer() const { return buffer; }

uint16_t SerialisedMessage::GetSize() const { return size; }
