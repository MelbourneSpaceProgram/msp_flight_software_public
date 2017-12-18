#ifndef SRC_MESSAGES_TEST_MESSAGE_H_
#define SRC_MESSAGES_TEST_MESSAGE_H_

#include <stdint.h>
#include "src/messages/message.h"

class TestMessage : public Message {
   public:
    explicit TestMessage(char data);
    virtual ~TestMessage() {}
    SerialisedMessage Serialise();

   private:
    char message_content;
};

#endif /* SRC_MESSAGES_TEST_MESSAGE_H_ */
