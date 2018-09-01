#ifndef SRC_PAYLOAD_PROCESSOR_COMMANDS_LITHIUM_SET_PA_COMMAND_H_
#define SRC_PAYLOAD_PROCESSOR_COMMANDS_LITHIUM_SET_PA_COMMAND_H_

#include <src/messages/LithiumFastPaCommandPayload.pb.h>
#include <src/payload_processor/commands/command.h>

class LithiumSetPaCommand : public Command {
   public:
    LithiumSetPaCommand(byte* payload);
    bool ExecuteCommand();

   private:
    LithiumFastPaCommandPayload pa_level;
};
#endif  // SRC_PAYLOAD_PROCESSOR_COMMANDS_LITHIUM_SET_PA_COMMAND_H_
