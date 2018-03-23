#include <src/payload_processor/commands/test_command.h>

TestCommand::TestCommand(byte* payload, uint8_t command_argument_start_index)
    : Command(kEchoCommandArgumentLength) {
    command_arguments[0] = payload[command_argument_start_index];
    command_arguments[1] = payload[command_argument_start_index + 1];
}

bool TestCommand::ExecuteCommand() {
    const byte write_buffer[kEchoCommandArgumentLength] = {
        command_arguments[0], command_arguments[1]};

    if (command_arguments[0] == kTestValue1 &&
        command_arguments[1] == kTestValue2) {
        return true;
    }
    return false;
}
