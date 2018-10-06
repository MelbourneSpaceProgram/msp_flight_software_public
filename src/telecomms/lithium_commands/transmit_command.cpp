#include <src/telecomms/lithium.h>
#include <src/telecomms/lithium_commands/lithium_command_codes.h>
#include <src/telecomms/lithium_commands/transmit_command.h>
#include <src/telecomms/fec/rs8.h>

TransmitCommand::TransmitCommand(TransmitPayload *transmit_payload)
    : transmit_packet(transmit_payload),
      LithiumCommand(kTransmitCommandCode, &transmit_packet) {}

byte *TransmitCommand::DoFec(byte *payload) {
	byte payload_parity[Rs8::kParityLength] = {0};
	byte payload_fec[Rs8::kBlockLength] = {0};
	Rs8::Encode(payload, payload_parity);
	memcpy(payload_fec, payload, Rs8::kDataLength)
	memcpy(&payload_fec[Rs8::kDataLength], payload_parity, Rs8::kParityLength);
	// Do stuff with payload_fec...
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
