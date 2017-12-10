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

class TestMessage: public Message {
public:
    TestMessage(char* bytes, uint8_t size);
    virtual ~TestMessage();
    SerialisedMessage Serialise();
private:
    char* message_content;
    uint8_t size;
};

#endif /* TESTMESSAGE_H_ */
