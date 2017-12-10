/*
 * TestMessage.h
 *
 *  Created on: 10Dec.,2017
 *      Author: Ben
 */

#ifndef TESTMESSAGE_H_
#define TESTMESSAGE_H_

#include <src/messages/Message.h>
#include <stdint.h>
#include <vector>

const uint8_t kTestMessageLength = 4;

class TestMessage: public Message {
public:
    TestMessage(char bytes[kTestMessageLength]);
    virtual ~TestMessage();
    SerialisedMessage Serialise();
private:
    char *message_content;
};

#endif /* TESTMESSAGE_H_ */
