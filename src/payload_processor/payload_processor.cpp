#include <src/payload_processor/commands/command.h>
#include <src/payload_processor/commands/force_reset_command.h>
#include <src/payload_processor/commands/format_sd_command.h>
#include <src/payload_processor/commands/lithium_beacon_period_command.h>
#include <src/payload_processor/commands/lithium_enable_command.h>
#include <src/payload_processor/commands/lithium_set_pa_command.h>
#include <src/payload_processor/commands/lithium_test_command.h>
#include <src/payload_processor/commands/test_command.h>
#include <src/payload_processor/commands/tle_update_command.h>
#include <src/payload_processor/commands/deploy_antenna_command.h>
#include <src/payload_processor/commands/enable_datalogger_command.h>
#include <src/payload_processor/payload_processor.h>
#include <src/telecomms/lithium.h>
#include <xdc/runtime/Log.h>

PayloadProcessor::PayloadProcessor() {}

bool PayloadProcessor::ParseAndExecuteCommands(byte* payload) {
    byte current_index = 0;

    while (current_index < Lithium::kMaxReceivedUplinkSize &&
           !(payload[current_index] == kEndTerminator &&
             payload[current_index + 1] == kEndTerminator)) {
        bool command_execution_successful =
            ParseNextCommandAndExecute(current_index, payload);

        if (!command_execution_successful) {
            // Command not found. Without a command, the rest of the payload
            // cannot be parsed.
            return false;
        } else {
            Lithium::command_success_count =
                Lithium::command_success_count == 255
                    ? 0
                    : Lithium::command_success_count + 1;
        }
    }
    return true;
}

bool PayloadProcessor::ParseNextCommandAndExecute(uint8_t& index,
                                                  byte* payload) {
    uint16_t command_code = static_cast<uint16_t>(payload[index]) |
                            (static_cast<uint16_t>(payload[index + 1]) << 8);
    Command* command = NULL;
    bool command_execution_successful = false;

    payload += kCommandCodeLength;

    try {
        // TODO(akremor): Is this referencing a local variable?
        switch (command_code) {
            case kTestCommand:
                TestCommand test_command(payload);
                command = &test_command;
                break;
            case kLithiumEnableCommand:
                LithiumEnableCommand lithium_enable_command(payload);
                command = &lithium_enable_command;
                break;
            case kTleUpdateCommand:
                TleUpdateCommand tle_update_command(payload);
                command = &tle_update_command;
                break;
            case kForceResetCommand:
                ForceResetCommand force_reset_command(payload);
                command = &force_reset_command;
                break;
            case kLithiumBeaconPeriodCommand:
                LithiumBeaconPeriodCommand beacon_period_command(payload);
                command = &beacon_period_command;
                break;
            case kLithiumFastPaCommand:
                LithiumSetPaCommand set_pa_command(payload);
                command = &set_pa_command;
                break;
            case kLithiumTestCommand:
                LithiumTestCommand lithium_test_command;
                command = &lithium_test_command;
                break;
            case kDeployAntennaCommand:
                DeployAntennaCommmand deploy_antenna_command(payload);
                command = &deploy_antenna_command;
                break;
            case kEnableDataloggerCommand:
                EnableDataloggerCommand enable_datalogger_command(payload);
                command = &enable_datalogger_command;
                break;
            case kFormatSdCommand:
                FormatSdCommand format_sd_command(payload);
                command = &format_sd_command;
                break;
            default:
                etl::exception e("Payload command does not exist!", __FILE__,
                                 __LINE__);
                throw e;
                break;
        }

        try {
            if (command != NULL) {
                command_execution_successful = command->ExecuteCommand();
                index += command->GetCommandArgumentLength() + kCommandCodeLength;
            }
        } catch (etl::exception e) {
            Log_error1("Unable to successfully execute command with code %d",
                       command_code);
            // TODO(akremor): Possible failure mode needs to be handled
        }
    } catch (etl::exception e) {
        Log_error1("Could not parse command with code code %d", command_code);
        // TODO(akremor): Possible failure mode needs to be handled
    }

    return command_execution_successful;
}

byte PayloadProcessor::GetCommandCodeLength() { return kCommandCodeLength; }

byte PayloadProcessor::GetEndTerminator() { return kEndTerminator; }
