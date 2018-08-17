#include <src/messages/serialised_message_builder.h>
#include <src/telecomms/lithium.h>
#include <src/telecomms/msp_payloads/test_ones_payload.h>
#include <src/util/message_codes.h>

TestOnesPayload::TestOnesPayload() {}

SerialisedMessage TestOnesPayload::SerialiseTo(byte *serial_buffer) const {
    SerialisedMessageBuilder builder(serial_buffer, GetSerialisedSize());
    builder.PadZeroes();
    return builder.Build();
}

uint16_t TestOnesPayload::GetSerialisedSize() const { return kPayloadSize; }

uint8_t TestOnesPayload::GetPayloadCode() const { return kTestOnesPayloadCode; }
