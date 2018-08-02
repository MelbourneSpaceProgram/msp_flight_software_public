#include <src/messages/serialised_message_builder.h>
#include <src/telecomms/lithium.h>
#include <src/telecomms/msp_payloads/transmit_packet.h>
#include <src/telecomms/msp_payloads/transmit_payload.h>

TransmitPacket::TransmitPacket(TransmitPayload *transmit_payload)
    : tx_count(Lithium::GetTxCounter()),
      rx_count(Lithium::GetRxCounter()),
      command_success_count(Lithium::GetCommandSuccessCounter()),
      transmit_payload(transmit_payload) {}

SerialisedMessage TransmitPacket::SerialiseTo(byte *serial_buffer) const {
    SerialisedMessageBuilder builder(serial_buffer, GetSerialisedSize());
    builder.AddData<uint8_t>(tx_count)
        .AddData<uint8_t>(rx_count)
        .AddData<uint8_t>(command_success_count)
        .AddData<uint8_t>(transmit_payload->GetPayloadCode())
        .AddMessage(transmit_payload);

    return builder.Build();
}

uint16_t TransmitPacket::GetSerialisedSize() const {
    return kMspHeaderSize + transmit_payload->GetSerialisedSize();
}
