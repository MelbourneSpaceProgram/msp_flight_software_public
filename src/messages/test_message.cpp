#include <src/CDH/util/serialised_message_builder.h>
#include "src/messages/test_message.h"

TestMessage::TestMessage(char data) {
    message_content = data;
}

TestMessage::~TestMessage() {
    // Auto-generated destructor stub
}

SerialisedMessage TestMessage::Serialise() {
    SerialisedMessageBuilder builder(SerialisedMessageBuilder::kTestSensor, SerialisedMessageBuilder::kV1);
    builder.AddData<char>(message_content);
    return builder.Build();
}
