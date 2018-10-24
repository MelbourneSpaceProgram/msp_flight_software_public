#ifndef COMMANDS_LITHIUM_TEST_COMMAND_H_
#define COMMANDS_LITHIUM_TEST_COMMAND_H_

#include <src/payload_processor/commands/command.h>

class LithiumTestCommand : public Command {
   public:
    LithiumTestCommand();
    bool ExecuteCommand();

   private:
    static constexpr uint8_t kLithiumTestCommandArgumentLength = 1;
    static constexpr uint8_t kNumTestTransmits = 10;
};
#endif  // SRC_PAYLOAD_PROCESSOR_COMMANDS_LITHIUM_TEST_COMMAND_H_
