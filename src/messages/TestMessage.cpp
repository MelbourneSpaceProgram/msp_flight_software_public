#include <src/CDH/util/SerialisedMessageBuilder.h>
#include <src/messages/TestMessage.h>

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
