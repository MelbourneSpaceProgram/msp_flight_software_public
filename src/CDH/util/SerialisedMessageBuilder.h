/*
 * SerialisedMessageBuilder.h
 *
 *  Created on: 24Nov.,2017
 *      Author: Anthony
 */

#ifndef SERIALISEDMESSAGEBUILDER_H_
#define SERIALISEDMESSAGEBUILDER_H_

#include <src/messages/SerialisedMessage.h>

#define BUFFER_SIZE 256

class SerialisedMessageBuilder {
public:
    enum SerialisedMessageType {TEMPERATURE_SENSOR = 0,
                                   RADIATION_SENSOR = 1};
    enum SerialisedMessageVersion {V1 = 0};
    SerialisedMessageBuilder(SerialisedMessageType type, SerialisedMessageVersion version);
    virtual ~SerialisedMessageBuilder();
    int getSerialisedLength();
    char *getSerialisedMessageBuffer();
    SerialisedMessage build();
    template <class T>
    SerialisedMessageBuilder& addData(T data) {
        if(sizeof(T) > BUFFER_SIZE - serialisedLength) {
            // TODO: Throw and handle exception
        } else {
            serialisedLength += sizeof(T);
            std::memcpy(serialisedMessageBuffer + serialisedLength, &data, sizeof(T));
        }
        return *this;
    }
private:
    int serialisedLength;
    char serialisedMessageBuffer[BUFFER_SIZE];
};

#endif /* SERIALISEDMESSAGEBUILDER_H_ */
