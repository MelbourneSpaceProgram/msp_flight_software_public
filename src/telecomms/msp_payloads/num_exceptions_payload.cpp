#include <src/messages/serialised_message.h>
#include <src/messages/serialised_message_builder.h>
#include <src/telecomms/msp_payloads/num_exceptions_payload.h>
#include <src/util/message_codes.h>
#include <src/util/msp_exception.h>

NumExceptionsPayload::NumExceptionsPayload() {}

SerialisedMessage NumExceptionsPayload::SerialiseTo(byte* serial_buffer) const {
    SerialisedMessageBuilder builder(serial_buffer, GetSerialisedSize());

    const uint8_t* retrieved = MspException::GetNumAll();
    for (uint8_t i = 0; i < kNumExceptionTypes; ++i) {
        builder.AddData<uint8_t>(retrieved[i]);
    }
    return builder.Build();
}

uint16_t NumExceptionsPayload::GetSerialisedSize() const {
    return kSerialisedSize;
}

uint8_t NumExceptionsPayload::GetPayloadCode() const {
    return kNumExceptionsPayloadCode;
}
