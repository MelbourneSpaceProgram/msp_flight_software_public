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

void PackWriter::addInt(int value){

    // Now work out how many bytes to bring in
    // TODO I don't think this bothers about endianess, which is bad

    // TODO Need some error bounds on 256
    for(int i = sizeof(value) - 1; i >= 0; i--){
        packed_message_buffer[packed_length] = (value >> (8 * i)) & 0xff;
        packed_length++;
    }
}

void PackWriter::addFloat(float value){

    // Now work out how many bytes to bring in
    // TODO I don't think this bothers about endianess, which is bad

    // TODO Need some error bounds on 256
    for(int i = sizeof(value) - 1; i >= 0; i--){
        packed_message_buffer[packed_length] = (*(uint32_t *)(&value) >>  (8  * i)) & 0xFF;
        packed_length++;
    }
}
