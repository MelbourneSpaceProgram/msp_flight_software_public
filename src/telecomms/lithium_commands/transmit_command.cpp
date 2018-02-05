#include <src/telecomms/lithium_commands/lithium_command_codes.h>
#include <src/telecomms/lithium_commands/transmit_command.h>

TransmitCommand::TransmitCommand(TransmitPayload *transmit_payload,
                                 uint8_t tx_count, uint8_t total_rx_count,
                                 uint8_t valid_rx_count)
    : transmit_packet(transmit_payload, tx_count, total_rx_count,
                      valid_rx_count),
      LithiumCommand(kTransmitCommandCode, &transmit_packet) {}

uint16_t TransmitCommand::GetLithiumPayloadSize() const {
    return transmit_packet.GetSerialisedSize();
}

uint16_t TransmitCommand::GetReplyPayloadSize() const {
    return LithiumCommand::kNoPayload;
}

const byte &TransmitCommand::GetCommandCode() const {
    return kTransmitCommandCode;
}
