#ifndef SERIALISED_MESSAGE_BUILDER_H_
#define SERIALISED_MESSAGE_BUILDER_H_

#include <cstdio>
#include "src/messages/serialised_message.h"
#include "src/util/data_types.h"

const uint16_t kBufferSize = 256;

class SerialisedMessageBuilder {
public:
    enum SerialisedMessageType {kTemperatureSensor = 0,
                                   kRadiationSensor = 1,
                                   kTestSensor = 3};    //TODO: Should this enum necessarily be in this file?
    enum SerialisedMessageVersion {kV1 = 0};
    SerialisedMessageBuilder(SerialisedMessageType type, SerialisedMessageVersion version);
    virtual ~SerialisedMessageBuilder();
    int GetSerialisedLength();
    byte *GetSerialisedMessageBuffer();
    SerialisedMessage Build();
    template <class T>
    SerialisedMessageBuilder& AddData(T data) {
        if(sizeof(T) > kBufferSize - serialised_length) {
            // TODO: Throw and handle exception after exception system is determined
        } else {
            std::memcpy(serialised_message_buffer + serialised_length, &data, sizeof(T));
            serialised_length += sizeof(T);
        }
        return *this;
    }
private:
    int serialised_length;
    byte serialised_message_buffer[kBufferSize];
};

#endif /* SERIALISED_MESSAGE_BUILDER_H_ */
