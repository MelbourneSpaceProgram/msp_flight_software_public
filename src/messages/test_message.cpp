#include <src/messages/serialised_message_builder.h>
#include <src/messages/test_message.h>

TestMessage::TestMessage(byte data) : message_content(data) {}

SerialisedMessage TestMessage::SerialiseTo(byte *serial_buffer) const {
    SerialisedMessageBuilder builder(serial_buffer, GetSerialisedSize(),
                                     kMockTestSensor, kV1);
    builder.AddData<byte>(message_content);
    return builder.Build();
}

uint16_t TestMessage::GetSerialisedSize() const { return kSerialisedSize; }
