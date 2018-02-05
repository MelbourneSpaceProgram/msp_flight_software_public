#include <src/telecomms/lithium_commands/lithium_command_codes.h>
#include <src/telecomms/lithium_commands/no_op_command.h>

NoOpCommand::NoOpCommand() : LithiumCommand(kNoOpCommandCode, NULL) {}

uint16_t NoOpCommand::GetLithiumPayloadSize() const { return 0; }

const byte &NoOpCommand::GetCommandCode() const { return kNoOpCommandCode; }

uint16_t NoOpCommand::GetReplyPayloadSize() const {
    return LithiumCommand::kNoPayload;
}
