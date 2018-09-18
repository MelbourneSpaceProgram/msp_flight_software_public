#include <src/messages/serialised_message_builder.h>
#include <src/telecomms/lithium_commands/fast_pa_level.h>

FastPaLevel::FastPaLevel(uint8_t pa_level) : pa_level(pa_level) {}

SerialisedMessage FastPaLevel::SerialiseTo(byte* serial_buffer) const {
    SerialisedMessageBuilder builder(serial_buffer, GetSerialisedSize());
    builder.AddData<uint8_t>(pa_level);
    return builder.Build();
}

uint16_t FastPaLevel::GetSerialisedSize() const { return sizeof(uint8_t); }
