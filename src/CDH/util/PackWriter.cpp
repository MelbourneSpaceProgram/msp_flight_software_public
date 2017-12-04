/*
 * PackWriter.cpp
 *
 *  Created on: 24Nov.,2017
 *      Author: Anthony
 */

#include <src/CDH/util/PackWriter.h>
#include <stdint.h>

PackWriter::PackWriter(PackMessageType type, PackMessageVersion version) {
    packedLength = 0;
    packedMessageBuffer[0] = type;
    packedMessageBuffer[1] = version;
    packedLength = 2;
}

PackWriter::~PackWriter() {
    // TODO Auto-generated destructor stub
}

int PackWriter::getPackedLength() {
    return packedLength;
}

char* PackWriter::getPackedMessageBuffer() {
    return packedMessageBuffer;
}

