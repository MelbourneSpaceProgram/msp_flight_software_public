#include "src/messages/test_message.h"
#include <src/CDH/util/serialised_message_builder.h>

TestMessage::TestMessage(char data) { message_content = data; }

SerialisedMessage TestMessage::Serialise() {
    SerialisedMessageBuilder builder(SerialisedMessageBuilder::kTestSensor,
                                     SerialisedMessageBuilder::kV1);
    builder.AddData<char>(message_content);
    return builder.Build();
}
