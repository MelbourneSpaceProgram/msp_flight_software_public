#include <src/messages/serialised_message_builder.h>
#include <src/telecomms/lithium.h>
#include <src/telecomms/msp_payloads/transmit_packet.h>
#include <src/telecomms/msp_payloads/transmit_payload.h>
#include <src/telecomms/fec/rs8.h>
#include <xdc/runtime/Log.h>
#include <src/util/msp_exception.h>
#include <src/util/message_codes.h>
#include <src/config/satellite.h>

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

    if (kDownlinkFecEnabled) {
        try {
            EncodeWithFec(serial_buffer, &builder);
        } catch (MspException& e) {
            MspException::LogException(e, kTransmitPacketBuild);
        }
    }

    return builder.Build();
}

void TransmitPacket::EncodeWithFec(byte* buffer,
                                   SerialisedMessageBuilder* builder) const{
    if (builder->GetSerialisedMessageBufferSize() < Rs8::kBlockLength) {
        MspException e("Buffer size too small to allow FEC", kFecBufferTooSmall,
                       __FILE__, __LINE__);
        throw e;
    } else if (builder->GetSerialisedLength() > Rs8::kDataLength) {
        MspException e("Payload size exceeds FEC block size",
                       kPayloadExceedsFecBuffer, __FILE__, __LINE__);
        throw e;
    }

    builder->PadZeroes();
    Rs8::Encode(buffer, buffer + Rs8::kDataLength);
}

uint16_t TransmitPacket::GetSerialisedSize() const {
    return Rs8::kBlockLength;
}
