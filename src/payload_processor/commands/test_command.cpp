#include <src/payload_processor/commands/test_command.h>

TestCommand::TestCommand(byte* payload) : Command(kEchoCommandArgumentLength) {
    command_arguments[0] = payload[0];
    command_arguments[1] = payload[1];
}

bool TestCommand::ExecuteCommand() {
    if (command_arguments[0] == kTestValue1 &&
        command_arguments[1] == kTestValue2) {
        return true;
    }
    return false;
}
