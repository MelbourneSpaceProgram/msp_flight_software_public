#ifndef SRC_PAYLOAD_PROCESSOR_COMMANDS_FORCE_RESET_COMMAND_H_
#define SRC_PAYLOAD_PROCESSOR_COMMANDS_FORCE_RESET_COMMAND_H_

#include <src/payload_processor/commands/command.h>
#include <src/util/reset_management.h>
#include <src/messages/SystemResetCommand.pb.h>

class ForceResetCommand : public Command {
public:
    explicit ForceResetCommand(byte* payload);
    bool ExecuteCommand();
private:
    SystemResetCommand reset_enable;
    static const uint8_t kForceResetCommandLength = SystemResetCommand_size;
    static const uint32_t kResetCommandCodeKey = 0xfee1dead;
};

#endif  // SRC_PAYLOAD_PROCESSOR_COMMANDS_FORCE_RESET_COMMAND_H_
