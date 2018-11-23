#ifndef SRC_MESSAGES_SERIALISED_MESSAGE_BUILDER_H_
#define SRC_MESSAGES_SERIALISED_MESSAGE_BUILDER_H_

#include <external/etl/array.h>
#include <src/messages/message.h>
#include <src/messages/serialised_message.h>
#include <src/util/data_types.h>
#include <src/util/message_codes.h>
#include <src/util/msp_exception.h>
#include <string.h>

class SerialisedMessageBuilder {
   public:
    SerialisedMessageBuilder(byte* serial_buffer, uint16_t buffer_size);
    SerialisedMessageBuilder(byte* serial_buffer, uint16_t buffer_size,
                             DebugMessageType type,
                             DebugMessageVersion version);
    uint16_t GetSerialisedLength() const;
    byte* GetSerialisedMessageBuffer() const;
    SerialisedMessage Build();
    SerialisedMessageBuilder& AddMessage(const Message* message);
    void PadZeroes();
    void PadOnes();

    template <class T>
    SerialisedMessageBuilder& AddData(const T data) {
        if (static_cast<uint16_t>(sizeof(T)) >
            buffer_size - serialised_length) {
            throw MspException("Message builder buffer size overflow",
                               kSerialisedMessageBuilderOverflowFail, __FILE__,
                               __LINE__);
        } else {
            memcpy(serialised_message_buffer + serialised_length, &data,
                   sizeof(T));
            serialised_length += sizeof(T);
        }
        return *this;
    }

    template <typename T>
    SerialisedMessageBuilder& AddArray(const T* array, size_t size) {
        for (uint16_t i = 0; i < size; i++) {
            AddData<T>(array[i]);
        }
        return *this;
    }

    template <typename T, const size_t SIZE>
    SerialisedMessageBuilder& AddEtlArray(const etl::array<T, SIZE>& array) {
        for (uint16_t i = 0; i < SIZE; i++) {
            AddData<T>(array.at(i));
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
