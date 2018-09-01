#ifndef COMMANDS_LITHIUM_ENABLE_COMMAND_H_
#define COMMANDS_LITHIUM_ENABLE_COMMAND_H_

#include <external/etl/array.h>
#include <src/messages/LithiumEnableCommandPayload.pb.h>
#include <src/payload_processor/commands/command.h>

class LithiumEnableCommand : public Command {
   public:
    LithiumEnableCommand(byte* payload);
    bool ExecuteCommand();

   private:
    LithiumEnableCommandPayload lithium_state;
    static const byte kLithiumEnable = 0x01;
    static const byte kLithiumDisable = 0x00;
};
#endif  // SRC_PAYLOAD_PROCESSOR_COMMANDS_LITHIUM_ENABLE_COMMAND_H_
