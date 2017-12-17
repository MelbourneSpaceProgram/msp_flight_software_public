#include <stdint.h>
#include "src/CDH/util/serialised_message_builder.h"

SerialisedMessageBuilder::SerialisedMessageBuilder(SerialisedMessageType type, SerialisedMessageVersion version) {
    serialised_length = 0;
    serialised_message_buffer[0] = type;
    serialised_message_buffer[1] = version;
    serialised_length = 2;
}

int SerialisedMessageBuilder::GetSerialisedLength() {
    return serialised_length;
}

byte* SerialisedMessageBuilder::GetSerialisedMessageBuffer() {
    return serialised_message_buffer;
}

SerialisedMessage SerialisedMessageBuilder::Build() {
    return SerialisedMessage(serialised_length, serialised_message_buffer);
}

