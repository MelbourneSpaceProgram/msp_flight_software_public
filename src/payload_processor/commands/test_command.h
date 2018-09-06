#ifndef SRC_PAYLOAD_PROCESSOR_COMMANDS_TEST_COMMAND_H_
#define SRC_PAYLOAD_PROCESSOR_COMMANDS_TEST_COMMAND_H_

#include <external/etl/array.h>
#include <src/payload_processor/commands/command.h>

class TestCommand : public Command {
   public:
    TestCommand(byte* payload);
    bool ExecuteCommand();
    static const byte kTestValue1 = 0xFF;
    static const byte kTestValue2 = 0xA5;

   private:
    static const uint8_t kEchoCommandArgumentLength = 2;
    etl::array<byte, kEchoCommandArgumentLength> command_arguments;
};

#endif  //  SRC_PAYLOAD_PROCESSOR_COMMANDS_TEST_COMMAND_H_
