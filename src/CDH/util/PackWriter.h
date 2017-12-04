/*
 * PackWriter.h
 *
 *  Created on: 24Nov.,2017
 *      Author: Anthony
 */

#ifndef PACKWRITER_H_
#define PACKWRITER_H_

#define BUFFER_SIZE 256

class PackWriter
{
public:
    enum PackMessageType {TEMPERATURE_SENSOR = 0,
                                   RADIATION_SENSOR = 1};
    enum PackMessageVersion {V1 = 0};
    PackWriter(PackMessageType type, PackMessageVersion version);
    virtual ~PackWriter();
    int getPackedLength();
    char *getPackedMessageBuffer();
    template <class T>
    PackWriter& addData(T data) {
        if(sizeof(T) > BUFFER_SIZE - packed_length) {
            // TODO: Throw and handle exception
        } else {
            packed_length += sizeof(T);
            std::memcpy(&packed_message_buffer + packed_length, &data, sizeof(T));
            return *this;
        }
    }
private:
    int packed_length;
    char packed_message_buffer[BUFFER_SIZE];
};

#endif /* PACKWRITER_H_ */
