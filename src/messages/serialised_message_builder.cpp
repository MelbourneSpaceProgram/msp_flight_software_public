#include <src/messages/serialised_message_builder.h>
#include <stdint.h>

SerialisedMessageBuilder::SerialisedMessageBuilder(byte* serial_buffer,
                                                   uint16_t buffer_size)
    : buffer_size(buffer_size),
      serialised_message_buffer(serial_buffer),
      serialised_length(0) {}

SerialisedMessageBuilder::SerialisedMessageBuilder(byte* serial_buffer,
                                                   uint16_t buffer_size,
                                                   DebugMessageType type,
                                                   DebugMessageVersion version)
    : buffer_size(buffer_size),
      serialised_message_buffer(serial_buffer),
      serialised_length(0) {
    serialised_message_buffer[0] = type;
    serialised_message_buffer[1] = version;
    serialised_length = 2;
}

SerialisedMessageBuilder& SerialisedMessageBuilder::AddMessage(
    const Message* message) {
    uint16_t message_size = message->GetSerialisedSize();
    if (message_size > buffer_size - serialised_length) {
        // TODO(dingbenjamin): Throw and handle exception after exception
        // system is determined
    } else {
        // Serialise to the smallest unused sub-address of the buffer if the
        // condition is met that the serialised bytes will fit
        message->SerialiseTo(&serialised_message_buffer[serialised_length]);
        serialised_length += message_size;
    }
    return *this;
}

uint16_t SerialisedMessageBuilder::GetSerialisedLength() const {
    return serialised_length;
}

byte* SerialisedMessageBuilder::GetSerialisedMessageBuffer() const {
    return serialised_message_buffer;
}

SerialisedMessage SerialisedMessageBuilder::Build() {
    return SerialisedMessage(serialised_length, serialised_message_buffer);
}
