/*
 * PackWriter.h
 *
 *  Created on: 24Nov.,2017
 *      Author: Anthony
 */

#ifndef PACKWRITER_H_
#define PACKWRITER_H_

#define BUFFER_SIZE 256

class PackWriter {
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
        if(sizeof(T) > BUFFER_SIZE - packedLength) {
            // TODO: Throw and handle exception
        } else {
            packedLength += sizeof(T);
            std::memcpy(&packedMessageBuffer + packedLength, &data, sizeof(T));
            return *this;
        }
    }
private:
    int packedLength;
    char packedMessageBuffer[BUFFER_SIZE];
};

#endif /* PACKWRITER_H_ */
