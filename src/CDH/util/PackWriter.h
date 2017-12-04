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
    int packed_length;
    char packed_message_buffer[256];
    enum PackMessageType {TEMPERATURE_SENSOR = 0,
                                    RADIATION_SENSOR = 1};
    enum PackMessageVersion {V1 = 0};
    PackWriter(PackMessageType type, PackMessageVersion version);
    virtual ~PackWriter();
    template <class T>
    PackWriter& addData(T data) {
        // TODO: Prevent overpacking
        packed_length += sizeof(T);
        std::memcpy(&packed_message_buffer + packed_length, &data, sizeof(T));
        return *this;
    }
};

#endif /* PACKWRITER_H_ */
