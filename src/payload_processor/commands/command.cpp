#include <src/payload_processor/commands/command.h>

Command::Command(uint8_t command_arg_length)
    : command_arg_length(command_arg_length) {}

uint8_t Command::GetCommandArgumentLength() const { return command_arg_length; }
