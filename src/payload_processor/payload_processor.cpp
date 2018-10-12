#include <src/payload_processor/commands/command.h>
#include <src/payload_processor/commands/deploy_antenna_command.h>
#include <src/payload_processor/commands/enable_datalogger_command.h>
#include <src/payload_processor/commands/force_reset_command.h>
#include <src/payload_processor/commands/format_sd_command.h>
#include <src/payload_processor/commands/lithium_beacon_period_command.h>
#include <src/payload_processor/commands/lithium_enable_command.h>
#include <src/payload_processor/commands/lithium_set_pa_command.h>
#include <src/payload_processor/commands/lithium_test_command.h>
#include <src/payload_processor/commands/science_data_command.h>
#include <src/payload_processor/commands/test_command.h>
#include <src/payload_processor/commands/tle_update_command.h>
#include <src/payload_processor/payload_processor.h>
#include <src/telecomms/lithium.h>
#include <xdc/runtime/Log.h>
#include <memory>
#include <utility>

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

Command* PayloadProcessor::CreateCommand(uint16_t command_code, byte* payload) {
    switch (command_code) {
        case kTestCommand:
            return new TestCommand(payload);
        case kLithiumEnableCommand:
            return new LithiumEnableCommand(payload);
        case kTleUpdateCommand:
            return new TleUpdateCommand(payload);
        case kForceResetCommand:
            return new ForceResetCommand(payload);
        case kLithiumBeaconPeriodCommand:
            return new LithiumBeaconPeriodCommand(payload);
        case kLithiumFastPaCommand:
            return new LithiumSetPaCommand(payload);
        case kLithiumTestCommand:
            return new LithiumTestCommand;
        case kDeployAntennaCommand:
            return new DeployAntennaCommmand(payload);
        case kEnableDataloggerCommand:
            return new EnableDataloggerCommand(payload);
        case kFormatSdCommand:
            return new FormatSdCommand(payload);
        case kScienceDataCommand:
            return new ScienceDataCommand(payload);
        default:
            // TODO(dingbenjamin): Put erroneous command ID in exception
            etl::exception e("Could not parse command code", __FILE__,
                             __LINE__);
            throw e;
    }
}

bool PayloadProcessor::ParseNextCommandAndExecute(uint8_t& index,
                                                  byte* payload) {
    uint16_t command_code = static_cast<uint16_t>(payload[index]) |
                            (static_cast<uint16_t>(payload[index + 1]) << 8);
    payload += kCommandCodeLength;
    bool command_execution_successful = false;

    try {
        std::unique_ptr<Command> command(CreateCommand(command_code, payload));
        command_execution_successful = command->ExecuteCommand();
        index += command->GetCommandArgumentLength() + kCommandCodeLength;
    } catch (etl::exception& e) {
        // TODO(dingbenjamin): Figure out xdc_IArg
        Log_error1("Exception in payload processor: %s\n", (xdc_IArg)e.what());
    }
    return command_execution_successful;
}

byte PayloadProcessor::GetCommandCodeLength() { return kCommandCodeLength; }

byte PayloadProcessor::GetEndTerminator() { return kEndTerminator; }
