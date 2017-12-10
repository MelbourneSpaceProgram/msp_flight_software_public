/*
 * SerialisedMessageBuilder.cpp
 *
 *  Created on: 24Nov.,2017
 *      Author: Anthony, Ben
 */

#include <src/CDH/util/SerialisedMessageBuilder.h>
#include <stdint.h>

SerialisedMessageBuilder::SerialisedMessageBuilder(SerialisedMessageType type, SerialisedMessageVersion version) {
    serialised_length = 0;
    serialised_message_buffer[0] = type;
    serialised_message_buffer[1] = version;
    serialised_length = 2;
}

SerialisedMessageBuilder::~SerialisedMessageBuilder() {
    // TODO Auto-generated destructor stub
}

int SerialisedMessageBuilder::GetSerialisedLength() {
    return serialised_length;
}

char* SerialisedMessageBuilder::GetSerialisedMessageBuffer() {
    return serialised_message_buffer;
}

SerialisedMessage SerialisedMessageBuilder::Build() {
    return SerialisedMessage(serialised_length, serialised_message_buffer);
}

