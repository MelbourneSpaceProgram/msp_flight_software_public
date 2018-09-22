#include <src/messages/rebuildable_message_field_iterator.h>
#include <src/telecomms/lithium_commands/get_configuration_command.h>
#include <src/telecomms/lithium_commands/lithium_command_codes.h>

GetConfigurationCommand::GetConfigurationCommand()
    : LithiumResponseCommand<LithiumConfiguration>(kGetConfigurationCommandCode,
                                                   NULL) {}

uint16_t GetConfigurationCommand::GetLithiumPayloadSize() const { return 0; }

uint16_t GetConfigurationCommand::GetReplyPayloadSize() const {
    return GetConfigurationCommand::kGetConfigReplyPayloadSize;
}

const byte& GetConfigurationCommand::GetCommandCode() const {
    return kGetConfigurationCommandCode;
}

LithiumConfiguration GetConfigurationCommand::GetParsedResponse() {
    LithiumConfiguration parsed_configuration = {
        0, 0, 0, 0, 0, 0, 0, 0, {0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0},
        0, 0, 0, 0};

    RebuildableMessageFieldIterator iterator(GetReplyBuffer(),
                                             kGetConfigReplyPayloadSize);

    iterator.FetchData<uint16_t>(&parsed_configuration.function_config2)
        ->FetchData<uint16_t>(&parsed_configuration.function_config)
        ->FetchData<uint16_t>(&parsed_configuration.tx_postamble)
        ->FetchData<uint16_t>(&parsed_configuration.tx_preamble)
        ->FetchData<byte>(&parsed_configuration.destination[5])
        ->FetchData<byte>(&parsed_configuration.destination[4])
        ->FetchData<byte>(&parsed_configuration.destination[3])
        ->FetchData<byte>(&parsed_configuration.destination[2])
        ->FetchData<byte>(&parsed_configuration.destination[1])
        ->FetchData<byte>(&parsed_configuration.destination[0])
        ->FetchData<byte>(&parsed_configuration.source[5])
        ->FetchData<byte>(&parsed_configuration.source[4])
        ->FetchData<byte>(&parsed_configuration.source[3])
        ->FetchData<byte>(&parsed_configuration.source[2])
        ->FetchData<byte>(&parsed_configuration.source[1])
        ->FetchData<byte>(&parsed_configuration.source[0])
        ->FetchData<uint32_t>(&parsed_configuration.tx_freq)
        ->FetchData<uint32_t>(&parsed_configuration.rx_freq)
        ->FetchData<uint8_t>(&parsed_configuration.tx_modulation)
        ->FetchData<uint8_t>(&parsed_configuration.rx_modulation)
        ->FetchData<uint8_t>(&parsed_configuration.tx_rf_baud_rate)
        ->FetchData<uint8_t>(&parsed_configuration.rx_rf_baud_rate)
        ->FetchData<uint8_t>(&parsed_configuration.tx_power_amp_level)
        ->FetchData<uint8_t>(&parsed_configuration.interface_baud_rate);

        return parsed_configuration;
}
