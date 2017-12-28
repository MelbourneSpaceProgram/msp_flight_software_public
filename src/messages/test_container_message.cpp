#include <src/messages/serialised_message_builder.h>
#include <src/messages/test_container_message.h>
#include <src/util/message_codes.h>

TestContainerMessage::TestContainerMessage(byte base_data,
                                           Message *contained_message)
    : base_data(base_data), contained_message(contained_message) {}

SerialisedMessage TestContainerMessage::SerialiseTo(byte *serial_buffer) {
    SerialisedMessageBuilder builder(serial_buffer, GetSerialisedSize(),
                                     kMockTestContainer, kV1);
    builder.AddData<byte>(base_data);
    builder.AddMessage(contained_message);
    return builder.Build();
}

uint16_t TestContainerMessage::GetSerialisedSize() const {
    return kBaseSerialisedSize + contained_message->GetSerialisedSize();
}
