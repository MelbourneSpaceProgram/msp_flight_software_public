#include <src/messages/serialised_message_builder.h>
#include <src/telecomms/msp_payloads/test_payload.h>
#include <src/util/data_types.h>
#include <src/util/message_codes.h>

const char *TestPayload::kTestMessage =
    "This is a test message from the Melbourne Space Program";

TestPayload::TestPayload() {}

SerialisedMessage TestPayload::SerialiseTo(byte *serial_buffer) const {
    SerialisedMessageBuilder builder(serial_buffer, GetSerialisedSize());
    for (int i = 0; i < kTestMessageSize - 1; i++) {
        builder.AddData<char>(kTestMessage[i]);
    }
    // Add Carriage return
    builder.AddData<byte>(0x0d);
    return builder.Build();
}

uint16_t TestPayload::GetSerialisedSize() const { return kTestMessageSize; }

uint8_t TestPayload::GetPayloadCode() const { return kTestPayloadCode; }
