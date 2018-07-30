#include <src/telecomms/lithium_commands/fast_pa_command.h>

const byte FastPaCommand::kFastPaCommandCode;

FastPaCommand::FastPaCommand(uint8_t power_amp_level)
    : LithiumCommand(kFastPaCommandCode, &fast_power_amp_level),
      fast_power_amp_level(power_amp_level) {}

uint16_t FastPaCommand::GetLithiumPayloadSize() const {
    return sizeof(uint8_t);
}

const byte &FastPaCommand::GetCommandCode() const { return kFastPaCommandCode; }

uint16_t FastPaCommand::GetReplyPayloadSize() const {
    return LithiumCommand::kNoPayload;
}
