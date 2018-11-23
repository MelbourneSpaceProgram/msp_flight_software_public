#include <src/payload_processor/payload_processor.h>
#include <src/payload_processor/uplinks/terminate_antenna_burn_uplink.h>
#include <src/payload_processor/uplinks/clear_exceptions_uplink.h>
#include <src/payload_processor/uplinks/clear_sections_uplink.h>
#include <src/payload_processor/uplinks/deploy_antenna_uplink.h>
#include <src/payload_processor/uplinks/enable_datalogger_uplink.h>
#include <src/payload_processor/uplinks/erase_flash_uplink.h>
#include <src/payload_processor/uplinks/execute_sections_uplink.h>
#include <src/payload_processor/uplinks/force_reset_uplink.h>
#include <src/payload_processor/uplinks/format_sd_uplink.h>
#include <src/payload_processor/uplinks/io_expander_toggle_uplink.h>
#include <src/payload_processor/uplinks/lithium_beacon_period_uplink.h>
#include <src/payload_processor/uplinks/lithium_enable_uplink.h>
#include <src/payload_processor/uplinks/lithium_get_configuration_uplink.h>
#include <src/payload_processor/uplinks/lithium_set_configuration_uplink.h>
#include <src/payload_processor/uplinks/lithium_set_pa_uplink.h>
#include <src/payload_processor/uplinks/lithium_test_uplink.h>
#include <src/payload_processor/uplinks/lithium_write_flash_uplink.h>
#include <src/payload_processor/uplinks/query_exceptions_uplink.h>
#include <src/payload_processor/uplinks/query_num_exceptions_uplink.h>
#include <src/payload_processor/uplinks/query_sections_uplink.h>
#include <src/payload_processor/uplinks/science_data_uplink.h>
#include <src/payload_processor/uplinks/section_uplink.h>
#include <src/payload_processor/uplinks/test_uplink.h>
#include <src/payload_processor/uplinks/tle_update_uplink.h>
#include <src/payload_processor/uplinks/uplink.h>
#include <src/telecomms/lithium.h>
#include <src/util/msp_exception.h>
#include <stdint.h>
#include <xdc/runtime/Log.h>
#include <memory>
#include <utility>

PayloadProcessor::PayloadProcessor() : payload_section_manager() {}

bool PayloadProcessor::ParseAndExecuteUplinks(byte* payload) {
    byte current_index = 0;

    while (current_index < Lithium::kMaxReceivedUplinkSize &&
           !(payload[current_index] == kEndTerminator &&
             payload[current_index + 1] == kEndTerminator)) {
        bool command_execution_successful =
            ParseNextUplinkAndExecute(current_index, payload);

        if (!command_execution_successful) {
            // Uplink not found. Without a command, the rest of the payload
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

Uplink* PayloadProcessor::CreateUplink(uint16_t command_code, byte* payload) {
    switch (command_code) {
        case kTestUplink:
            return new TestUplink(payload);
        case kLithiumEnableUplink:
            return new LithiumEnableUplink(payload);
        case kTleUpdateUplink:
            return new TleUpdateUplink(payload);
        case kForceResetUplink:
            return new ForceResetUplink(payload);
        case kLithiumBeaconPeriodUplink:
            return new LithiumBeaconPeriodUplink(payload);
        case kLithiumFastPaUplink:
            return new LithiumSetPaUplink(payload);
        case kLithiumTestUplink:
            return new LithiumTestUplink;
        case kDeployAntennaUplink:
            return new DeployAntennaUplink(payload);
        case kEnableDataloggerUplink:
            return new EnableDataloggerUplink(payload);
        case kFormatSdUplink:
            return new FormatSdUplink(payload);
        case kScienceDataUplink:
            return new ScienceDataUplink(payload);
        case kIoExpanderToggleUplink:
            return new IoExpanderToggleUplink(payload);
        case kLithiumSetConfigurationUplink:
            return new LithiumSetConfigurationUplink(payload);
        case kLithiumGetConfigurationUplink:
            return new LithiumGetConfigurationUplink;
        case kLithiumWriteFlashUplink:
            return new LithiumWriteFlashUplink(payload);
        case kSectionUplink:
            return new SectionUplink(payload, this);
        case kClearSectionsUplink:
            return new ClearSectionsUplink(payload, this);
        case kExecuteSectionsUplink:
            return new ExecuteSectionsUplink(payload, this);
        case kQuerySectionsUplink:
            return new QuerySectionsUplink(payload, this);
        case kClearExceptionsUplink:
            return new ClearExceptionsUplink(payload);
        case kQueryExceptionsUplink:
            return new QueryExceptionsUplink(payload);
        case kQueryNumExceptionsUplink:
            return new QueryNumExceptionsUplink(payload);
        case kEraseFlashUplink:
            return new EraseFlashUplink(payload);
        case kTerminateAntennaBurnUplink:
            return new TerminateAntennaBurnUplink(payload);
        default:
            throw MspException("Could not parse command code",
                               kPayloadProcessorCommandCodeFail, __FILE__,
                               __LINE__, command_code % UINT8_MAX,
                               command_code << 8);
    }
}

bool PayloadProcessor::ParseNextUplinkAndExecute(uint8_t& index,
                                                 byte* payload) {
    uint16_t command_code = static_cast<uint16_t>(payload[index]) |
                            (static_cast<uint16_t>(payload[index + 1]) << 8);
    payload += kUplinkCodeLength + index;
    bool command_execution_successful = false;

    try {
        std::unique_ptr<Uplink> command(CreateUplink(command_code, payload));
        command_execution_successful = command->ExecuteUplink();
        index += command->GetUplinkArgumentLength() + kUplinkCodeLength;
    } catch (MspException& e) {
        Log_error0("Exception in payload processor:");
		MspException::LogException(e, kPayloadProcessorCatch);
    }
    return command_execution_successful;
}

byte PayloadProcessor::GetUplinkCodeLength() { return kUplinkCodeLength; }

byte PayloadProcessor::GetEndTerminator() { return kEndTerminator; }

PayloadSectionManager* PayloadProcessor::GetPayloadSectionManager() {
    return &payload_section_manager;
}
