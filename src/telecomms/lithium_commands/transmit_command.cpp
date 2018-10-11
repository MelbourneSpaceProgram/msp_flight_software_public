#include <src/telecomms/lithium.h>
#include <src/telecomms/lithium_commands/lithium_command_codes.h>
#include <src/telecomms/lithium_commands/transmit_command.h>
#include <src/telecomms/runnable_continuous_transmit_shutoff.h>

TransmitCommand::TransmitCommand(TransmitPayload *transmit_payload)
    : transmit_packet(transmit_payload),
      LithiumCommand(kTransmitCommandCode, &transmit_packet) {
    RunnableContinuousTransmitShutoff::IncrementBucketCount();
}

uint16_t TransmitCommand::GetLithiumPayloadSize() const {
    return transmit_packet.GetSerialisedSize();
}

uint16_t TransmitCommand::GetReplyPayloadSize() const {
    return LithiumCommand::kNoPayload;
}

const byte &TransmitCommand::GetCommandCode() const {
    return kTransmitCommandCode;
}
