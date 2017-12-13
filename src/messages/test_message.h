#ifndef TEST_MESSAGE_H_
#define TEST_MESSAGE_H_

#include <stdint.h>
#include "message.h"

class TestMessage: public Message {
public:
    TestMessage(char data);
    virtual ~TestMessage();
    SerialisedMessage Serialise();
private:
    char message_content;
};

#endif /* TEST_MESSAGE_H_ */
