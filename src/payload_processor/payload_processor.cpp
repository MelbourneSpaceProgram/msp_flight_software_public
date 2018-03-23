#include <src/payload_processor/commands/command.h>
#include <src/payload_processor/commands/test_command.h>
#include <src/payload_processor/payload_processor.h>
#include <src/telecomms/lithium.h>

bool PayloadProcessor::ParseAndExecuteCommands(byte* payload) {
    byte current_index = 0;

    while (current_index < Lithium::kMaxReceivedSize &&
           payload[current_index] != kEndTerminator) {
        bool command_execution_successful =
            ParseNextCommandAndExecute(current_index, payload);

        if (!command_execution_successful) {
            // Command not found. Without a command, the rest of the payload
            // cannot be parsed.
            return false;
        }
    }
    return true;
}

bool PayloadProcessor::ParseNextCommandAndExecute(byte& index, byte* payload) {
    byte command_byte = payload[index];
    Command* command = NULL;
    bool command_execution_successful = false;

    switch (command_byte) {
        case kEchoCommand:
            TestCommand echo_command(payload, index + kCommandLength);
            command = &echo_command;
            break;
    }

    if (command != NULL) {
        command_execution_successful = command->ExecuteCommand();
        index += command->GetCommandArgumentLength() + kCommandLength;
    }

    return command_execution_successful;
}
