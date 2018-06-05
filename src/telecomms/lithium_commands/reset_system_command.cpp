#include <src/telecomms/lithium_commands/reset_system_command.h>

const byte ResetSystemCommand::kResetSystemCommandCode;

ResetSystemCommand::ResetSystemCommand()
    : LithiumCommand(kResetSystemCommandCode, NULL) {}

uint16_t ResetSystemCommand::GetLithiumPayloadSize() const { return 0; }

const byte &ResetSystemCommand::GetCommandCode() const {
    return kResetSystemCommandCode;
}

uint16_t ResetSystemCommand::GetReplyPayloadSize() const {
    return LithiumCommand::kNoPayload;
}
