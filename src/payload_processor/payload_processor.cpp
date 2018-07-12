#include <src/payload_processor/commands/command.h>
#include <src/payload_processor/commands/force_reset_command.h>
#include <src/payload_processor/commands/lithium_enable_command.h>
#include <src/payload_processor/commands/test_command.h>
#include <src/payload_processor/commands/tle_update_command.h>
#include <src/payload_processor/payload_processor.h>
#include <src/telecomms/lithium.h>
#include <xdc/runtime/Log.h>

bool PayloadProcessor::ParseAndExecuteCommands(byte* payload) {
    byte current_index = 0;

    while (current_index < Lithium::kMaxReceivedSize &&
           !(payload[current_index] == kEndTerminator &&
             payload[current_index + 1] == kEndTerminator)) {
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
    uint16_t command_code = static_cast<uint16_t>(payload[index]) |
                            (static_cast<uint16_t>(payload[index + 1]) << 8);
    Command* command = NULL;
    bool command_execution_successful = false;

    try {
        // TODO(akremor): Is this referencing a local variable?
        switch (command_code) {
            case kEchoCommand:
                TestCommand echo_command(payload, index + kCommandCodeLength);
                command = &echo_command;
                break;
            case kLithiumEnableCommand:
                LithiumEnableCommand lithium_enable_command(payload +
                                                            kCommandCodeLength);
                command = &lithium_enable_command;
                break;
            case kTleUpdateCommand:
                TleUpdateCommand tle_update_command(payload,
                                                    index + kCommandCodeLength);
                command = &tle_update_command;
                break;
            case kForceResetCommand:
                ForceResetCommand force_reset_command;
                command = &force_reset_command;
                break;
        }

        try {
            if (command != NULL) {
                command_execution_successful = command->ExecuteCommand();
            }
        } catch (etl::exception e) {
            Log_error1("Unable to successfully execute command with code %d",
                       command_code);
            command_execution_successful = false;
            // TODO(akremor): Possible failure mode needs to be handled
        }
    } catch (etl::exception e) {
        Log_error1("Could not parse command with code code %d", command_code);
        // TODO(akremor): Possible failure mode needs to be handled
    }

    index += command->GetCommandArgumentLength() + kCommandCodeLength;

    return command_execution_successful;
}

byte PayloadProcessor::GetCommandCodeLength() { return kCommandCodeLength; }

byte PayloadProcessor::GetEndTerminator() { return kEndTerminator; }
