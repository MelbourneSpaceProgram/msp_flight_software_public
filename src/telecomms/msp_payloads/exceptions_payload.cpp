#include <src/messages/serialised_message.h>
#include <src/messages/serialised_message_builder.h>
#include <src/telecomms/msp_payloads/exceptions_payload.h>
#include <src/util/message_codes.h>
#include <src/util/msp_exception.h>

ExceptionsPayload::ExceptionsPayload(uint8_t error_id) : error_id(error_id) {}

SerialisedMessage ExceptionsPayload::SerialiseTo(byte* serial_buffer) const {
    SerialisedMessageBuilder builder(serial_buffer, GetSerialisedSize());

    const SerialisedException* retrieved =
        MspException::GetExceptions(error_id);
    for (uint8_t i = 0; i < MspException::kNumEachException; ++i) {
        builder.AddData<SerialisedException>(retrieved[i]);
    }
    return builder.Build();
}

uint16_t ExceptionsPayload::GetSerialisedSize() const {
    return kSerialisedSize;
}

uint8_t ExceptionsPayload::GetPayloadCode() const {
    return kExceptionsPayloadCode;
}
