#ifndef SRC_MESSAGES_SERIALISED_MESSAGE_BUILDER_H_
#define SRC_MESSAGES_SERIALISED_MESSAGE_BUILDER_H_

#include <src/messages/message.h>
#include <src/messages/serialised_message.h>
#include <src/util/data_types.h>
#include <src/util/message_codes.h>

class SerialisedMessageBuilder {
   public:
    SerialisedMessageBuilder(byte* serial_buffer, uint16_t buffer_size);
    SerialisedMessageBuilder(byte* serial_buffer, uint16_t buffer_size,
                             DebugMessageType type,
                             DebugMessageVersion version);
    uint16_t GetSerialisedLength() const;
    byte* GetSerialisedMessageBuffer() const;
    SerialisedMessage Build();
    SerialisedMessageBuilder& AddMessage(Message* message);
    template <class T>
    SerialisedMessageBuilder& AddData(T data) {
        if (sizeof(T) > buffer_size - serialised_length) {
            // TODO(dingbenjamin): Throw and handle exception after exception
            // system is determined
        } else {
            std::memcpy(serialised_message_buffer + serialised_length, &data,
                        sizeof(T));
            serialised_length += sizeof(T);
        }
        return *this;
    }

   private:
    SerialisedMessageBuilder(const SerialisedMessageBuilder&);
    SerialisedMessageBuilder& operator=(const SerialisedMessageBuilder&);
    uint16_t buffer_size;
    byte* serialised_message_buffer;
    uint16_t serialised_length;
};

#endif  // SRC_MESSAGES_SERIALISED_MESSAGE_BUILDER_H_
