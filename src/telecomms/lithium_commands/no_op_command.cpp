#include <src/telecomms/lithium_commands/no_op_command.h>
#include <src/util/data_types.h>

NoOpCommand::NoOpCommand() : LithiumCommand(kNoOpCommandCode, NULL) {}

uint16_t NoOpCommand::GetLithiumPayloadSize() const { return 0; }
