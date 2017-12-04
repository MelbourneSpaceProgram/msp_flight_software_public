/*
 * SerialisedMessageBuilder.cpp
 *
 *  Created on: 24Nov.,2017
 *      Author: Anthony
 */

#include <src/CDH/util/SerialisedMessageBuilder.h>
#include <stdint.h>

SerialisedMessageBuilder::SerialisedMessageBuilder(SerialisedMessageType type, SerialisedMessageVersion version) {
    serialisedLength = 0;
    serialisedMessageBuffer[0] = type;
    serialisedMessageBuffer[1] = version;
    serialisedLength = 2;
}

SerialisedMessageBuilder::~SerialisedMessageBuilder() {
    // TODO Auto-generated destructor stub
}

int SerialisedMessageBuilder::getSerialisedLength() {
    return serialisedLength;
}

char* SerialisedMessageBuilder::getSerialisedMessageBuffer() {
    return serialisedMessageBuffer;
}

SerialisedMessage SerialisedMessageBuilder::build() {
    return SerialisedMessage(serialisedLength, serialisedMessageBuffer);
}

