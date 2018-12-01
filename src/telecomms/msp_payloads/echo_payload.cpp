#include <src/messages/serialised_message_builder.h>
#include <src/telecomms/lithium.h>
#include <src/util/message_codes.h>
#include <src/telecomms/msp_payloads/echo_payload.h>
#include <string.h>
#include <xdc/runtime/Log.h>

EchoPayload::EchoPayload(byte* echo_message_in) {
    memcpy(&echo_message.at(0), echo_message_in, kEchoMessageLength * sizeof(byte));
    echo_message[kEchoMessageBufferLength - 1] = '\0';

    Log_info1("Received echo: %s", (IArg)(&echo_message.at(0)));
}

SerialisedMessage EchoPayload::SerialiseTo(byte *serial_buffer) const {
    SerialisedMessageBuilder builder(serial_buffer, GetSerialisedSize());
    for (int i = 0; i < kEchoMessageLength; i++) {
        builder.AddData<char>(echo_message[i]);
    }
    builder.PadZeroes();
    return builder.Build();
}

uint16_t EchoPayload::GetSerialisedSize() const { return kEchoMessageLength; }

uint8_t EchoPayload::GetPayloadCode() const { return kEchoPayloadCode; }
