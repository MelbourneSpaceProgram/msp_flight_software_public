#include <src/messages/serialised_message_builder.h>
#include <src/telecomms/lithium_commands/set_config_message.h>

SetConfigurationMessage::SetConfigurationMessage(LithiumConfiguration lithium_config)
    : lithium_config(lithium_config) {}

SerialisedMessage SetConfigurationMessage::SerialiseTo(
    byte* serial_buffer) const {
    SerialisedMessageBuilder builder(serial_buffer, GetSerialisedSize());

    builder.AddData<uint8_t>(lithium_config.interface_baud_rate)
        .AddData<uint8_t>(lithium_config.tx_power_amp_level)
        .AddData<uint8_t>(lithium_config.rx_rf_baud_rate)
        .AddData<uint8_t>(lithium_config.tx_rf_baud_rate)
        .AddData<uint8_t>(lithium_config.rx_modulation)
        .AddData<uint8_t>(lithium_config.tx_modulation)
        .AddData<uint32_t>(lithium_config.rx_freq)
        .AddData<uint32_t>(lithium_config.tx_freq)
        .AddData<byte>(lithium_config.source[0])
        .AddData<byte>(lithium_config.source[1])
        .AddData<byte>(lithium_config.source[2])
        .AddData<byte>(lithium_config.source[3])
        .AddData<byte>(lithium_config.source[4])
        .AddData<byte>(lithium_config.source[5])
        .AddData<byte>(lithium_config.destination[0])
        .AddData<byte>(lithium_config.destination[1])
        .AddData<byte>(lithium_config.destination[2])
        .AddData<byte>(lithium_config.destination[3])
        .AddData<byte>(lithium_config.destination[4])
        .AddData<byte>(lithium_config.destination[5])
        .AddData<uint16_t>(lithium_config.tx_preamble)
        .AddData<uint16_t>(lithium_config.tx_postamble)
        .AddData<uint16_t>(lithium_config.function_config)
        .AddData<uint16_t>(lithium_config.function_config2);

    return builder.Build();
}

uint16_t SetConfigurationMessage::GetSerialisedSize() const {
    return kLithiumConfigStructSize;
}
