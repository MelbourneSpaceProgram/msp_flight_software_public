#ifndef SRC_PAYLOAD_PROCESSOR_COMMANDS_ENABLE_DATALOGGER_COMMAND_H_
#define SRC_PAYLOAD_PROCESSOR_COMMANDS_ENABLE_DATALOGGER_COMMAND_H_

#include <src/messages/EnableDataloggerCommandPayload.pb.h>
#include <src/payload_processor/commands/command.h>

class EnableDataloggerCommand : public Command {
   public:
    explicit EnableDataloggerCommand(byte* payload);
    bool ExecuteCommand();

   private:
    EnableDataloggerCommandPayload datalogger_enabled;
};

#endif  // SRC_PAYLOAD_PROCESSOR_COMMANDS_ENABLE_DATALOGGER_COMMAND_H_
