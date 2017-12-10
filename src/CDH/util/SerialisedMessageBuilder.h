/*
 * SerialisedMessageBuilder.h
 *
 *  Created on: 24Nov.,2017
 *      Author: Anthony, Ben
 */

#ifndef SERIALISEDMESSAGEBUILDER_H_
#define SERIALISEDMESSAGEBUILDER_H_

#include <src/messages/SerialisedMessage.h>

const uint16_t kBufferSize = 256;

class SerialisedMessageBuilder {
public:
    enum SerialisedMessageType {kTemperatureSensor = 0,
                                   kRadiationSensor = 1};
    enum SerialisedMessageVersion {kV1 = 0};
    SerialisedMessageBuilder(SerialisedMessageType type, SerialisedMessageVersion version);
    virtual ~SerialisedMessageBuilder();
    int GetSerialisedLength();
    char *GetSerialisedMessageBuffer();
    SerialisedMessage Build();
    template <class T>
    SerialisedMessageBuilder& AddData(T data) {
        if(sizeof(T) > kBufferSize - serialised_length) {
            // TODO: Throw and handle exception
        } else {
            serialised_length += sizeof(T);
            std::memcpy(serialised_message_buffer + serialised_length, &data, sizeof(T));
        }
        return *this;
    }
private:
    int serialised_length;
    char serialised_message_buffer[kBufferSize];
};

#endif /* SERIALISEDMESSAGEBUILDER_H_ */
