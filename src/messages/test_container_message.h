#ifndef TEST_CONTAINER_MESSAGE_H_
#define TEST_CONTAINER_MESSAGE_H_

#include <src/messages/message.h>
#include <src/util/data_types.h>

class TestContainerMessage : public Message {
   public:
    explicit TestContainerMessage(byte base_data, Message *contained_message);
    SerialisedMessage SerialiseTo(byte *serial_buffer);
    uint16_t GetSerialisedSize() const;

   private:
    static const uint16_t kBaseSerialisedSize = 3;
    byte base_data;
    Message *contained_message;
};

#endif  // TEST_CONTAINER_MESSAGE_H_
