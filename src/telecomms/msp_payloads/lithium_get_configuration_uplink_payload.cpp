#include <src/messages/serialised_message_builder.h>
#include <src/telecomms/lithium.h>
#include <src/telecomms/msp_payloads/lithium_get_configuration_uplink_payload.h>
#include <src/util/data_types.h>
#include <src/util/message_codes.h>

LithiumGetConfigurationUplinkPayload::LithiumGetConfigurationUplinkPayload(
    LithiumConfiguration config)
    : config(config) {}

SerialisedMessage LithiumGetConfigurationUplinkPayload::SerialiseTo(
    byte *serial_buffer) const {
    SerialisedMessageBuilder builder(serial_buffer, GetSerialisedSize());

    builder.AddData<uint8_t>(config.interface_baud_rate)
        .AddData<uint8_t>(config.interface_baud_rate)
        .AddData<uint8_t>(config.tx_power_amp_level)
        .AddData<uint8_t>(config.rx_rf_baud_rate)
        .AddData<uint8_t>(config.tx_rf_baud_rate)
        .AddData<uint8_t>(config.rx_modulation)
        .AddData<uint8_t>(config.tx_modulation)
        .AddData<uint32_t>(config.rx_freq)
        .AddData<uint32_t>(config.tx_freq)
        .AddData<byte>(config.source[0])
        .AddData<byte>(config.source[1])
        .AddData<byte>(config.source[2])
        .AddData<byte>(config.source[3])
        .AddData<byte>(config.source[4])
        .AddData<byte>(config.source[5])
        .AddData<byte>(config.destination[0])
        .AddData<byte>(config.destination[1])
        .AddData<byte>(config.destination[2])
        .AddData<byte>(config.destination[3])
        .AddData<byte>(config.destination[4])
        .AddData<byte>(config.destination[5])
        .AddData<uint16_t>(config.tx_preamble)
        .AddData<uint16_t>(config.tx_postamble)
        .AddData<uint16_t>(config.function_config)
        .AddData<uint16_t>(config.function_config2);
    return builder.Build();
}

uint16_t LithiumGetConfigurationUplinkPayload::GetSerialisedSize() const {
    return kPayloadSize;
}

uint8_t LithiumGetConfigurationUplinkPayload::GetPayloadCode() const {
    return kLithiumGetConfigurationUplinkPayloadCode;
}
