#include "pad_zeroes_payload.h"

#include <src/messages/serialised_message_builder.h>
#include <src/telecomms/lithium.h>
#include <src/util/message_codes.h>

PadZeroesPayload::PadZeroesPayload() {}

SerialisedMessage PadZeroesPayload::SerialiseTo(byte *serial_buffer) const {
    SerialisedMessageBuilder builder(serial_buffer, GetSerialisedSize());
    builder.PadZeroes();
    return builder.Build();
}

uint16_t PadZeroesPayload::GetSerialisedSize() const { return kPayloadSize; }

uint8_t PadZeroesPayload::GetPayloadCode() const { return kTestOnesPayloadCode; }
