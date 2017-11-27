/*
 * PackWriter.h
 *
 *  Created on: 24Nov.,2017
 *      Author: Anthony
 */

#ifndef PACKWRITER_H_
#define PACKWRITER_H_

class PackWriter
{
public:
    enum PackMessageType {TEMPERATURE_SENSOR = 0,
                                    RADIATION_SENSOR = 1};
    enum PackMessageVersion {V1 = 0};
    PackWriter(PackMessageType type, PackMessageVersion version);
    virtual ~PackWriter();
    void addInt(int);
    void addFloat(float);
    void addChar(char);
    int packed_length;
    char packed_message_buffer[256];
    union Data {
        float f;         // Assigning fVal.f will also populate fVal.bytes;
        int i;
        char c;
        char bytes[4];   // Both fVal.f and fVal.bytes share the same 4 bytes of memory.
    };
};

#endif /* PACKWRITER_H_ */
