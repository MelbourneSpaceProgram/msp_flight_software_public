#ifndef SRC_MESSAGES_TEST_MESSAGE_H_
#define SRC_MESSAGES_TEST_MESSAGE_H_

#include <src/messages/message.h>
#include <src/util/data_types.h>
#include <stdint.h>

class TestMessage : public Message {
   public:
    explicit TestMessage(byte data);
    virtual ~TestMessage() {}
    SerialisedMessage SerialiseTo(byte *serial_buffer) const;
    uint16_t GetSerialisedSize() const;

   private:
    static const uint16_t kSerialisedSize = 3;
    byte message_content;
};

#endif  // SRC_MESSAGES_TEST_MESSAGE_H_
