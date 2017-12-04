/*
 * PackWriter.cpp
 *
 *  Created on: 24Nov.,2017
 *      Author: Anthony
 */

#include <src/CDH/util/PackWriter.h>
#include <stdint.h>

PackWriter::PackWriter(PackMessageType type, PackMessageVersion version)
{
    packed_length = 0;
    packed_message_buffer[0] = type;
    packed_message_buffer[1] = version;
    packed_length = 2;
}

PackWriter::~PackWriter()
{
    // TODO Auto-generated destructor stub
}

int PackWriter::getPackedLength() {
    return packed_length;
}

char* PackWriter::getPackedMessageBuffer() {
    return packed_message_buffer;
}

