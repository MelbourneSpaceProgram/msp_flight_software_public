#ifndef SRC_PAYLOAD_PROCESSOR_COMMANDS_FORCE_RESET_COMMAND_H_
#define SRC_PAYLOAD_PROCESSOR_COMMANDS_FORCE_RESET_COMMAND_H_

#include <src/payload_processor/commands/command.h>
#include <src/util/reset_management.h>

class ForceResetCommand : public Command {
public:
    ForceResetCommand();
    bool ExecuteCommand();
private:
    static const uint8_t kForceResetCommandLength = 0;
};

#endif  // SRC_PAYLOAD_PROCESSOR_COMMANDS_FORCE_RESET_COMMAND_H_
