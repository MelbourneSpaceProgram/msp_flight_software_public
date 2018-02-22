#include <src/messages/serialised_message_builder.h>
#include <src/telecomms/msp_payloads/transmit_packet.h>

TransmitPacket::TransmitPacket(TransmitPayload *transmit_payload,
                               uint8_t tx_count, uint8_t total_rx_count,
                               uint8_t valid_rx_count) {
    this->tx_count = tx_count;
    this->total_rx_count = total_rx_count;
    this->valid_rx_count = valid_rx_count;
    this->transmit_payload = transmit_payload;
}

SerialisedMessage TransmitPacket::SerialiseTo(byte *serial_buffer) const {
    SerialisedMessageBuilder builder(serial_buffer, GetSerialisedSize());
    builder.AddData<uint8_t>(tx_count)
        .AddData<uint8_t>(total_rx_count)
        .AddData<uint8_t>(valid_rx_count)
        .AddData<uint8_t>(transmit_payload->GetPayloadCode())
        .AddMessage(transmit_payload);

    return builder.Build();
}

uint16_t TransmitPacket::GetSerialisedSize() const {
    return kMspHeaderSize + transmit_payload->GetSerialisedSize();
}
