/*
 * TestMessage.cpp
 *
 *  Created on: 10Dec.,2017
 *      Author: Ben
 */

#include <src/CDH/util/SerialisedMessageBuilder.h>
#include <src/messages/TestMessage.h>



TestMessage::TestMessage(char bytes[kTestMessageLength]) {
    message_content = bytes;
}

TestMessage::~TestMessage() {
    // TODO Auto-generated destructor stub
}

SerialisedMessage TestMessage::Serialise() {
    SerialisedMessageBuilder builder(SerialisedMessageBuilder::kTestSensor, SerialisedMessageBuilder::kV1);
    for(uint8_t i = 0; i < kTestMessageLength; i++) {
        builder.AddData<char>(message_content[i]);
    }
    return builder.Build();
}
