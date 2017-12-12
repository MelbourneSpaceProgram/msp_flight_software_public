#ifndef TESTMESSAGE_H_
#define TESTMESSAGE_H_

#include <src/messages/Message.h>
#include <stdint.h>

class TestMessage: public Message {
public:
    TestMessage(char data);
    virtual ~TestMessage();
    SerialisedMessage Serialise();
private:
    char message_content;
};

#endif /* TESTMESSAGE_H_ */
