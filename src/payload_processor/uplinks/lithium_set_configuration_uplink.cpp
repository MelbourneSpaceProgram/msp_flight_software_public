#include <src/payload_processor/uplinks/lithium_set_configuration_uplink.h>
#include <src/telecomms/lithium.h>
#include <src/telecomms/lithium_commands/set_configuration_command.h>
#include <src/util/nanopb_utils.h>

LithiumSetConfigurationUplink::LithiumSetConfigurationUplink(byte* payload)
    : Uplink(LithiumConfigurationPayload_size),
      decoded_payload(NanopbDecode(LithiumConfigurationPayload)(payload)) {}

bool LithiumSetConfigurationUplink::ExecuteUplink() {
    LithiumConfiguration lithium_config;

    // TODO(dingbenjamin): Explicitly discard higher order bits in truncation
    lithium_config.interface_baud_rate =
        decoded_payload.interface_baud_rate_8 % 256;
    lithium_config.tx_power_amp_level =
        decoded_payload.tx_power_amp_level_8 % 256;
    lithium_config.rx_rf_baud_rate = decoded_payload.rx_rf_baud_rate_8 % 256;
    lithium_config.tx_rf_baud_rate = decoded_payload.tx_rf_baud_rate_8 % 256;
    lithium_config.rx_modulation = decoded_payload.rx_modulation_8 % 256;
    lithium_config.tx_modulation = decoded_payload.tx_modulation_8 % 256;
    lithium_config.rx_freq = decoded_payload.rx_freq_32;
    lithium_config.tx_freq = decoded_payload.tx_freq_32;
    lithium_config.source[0] = decoded_payload.source[0];
    lithium_config.source[1] = decoded_payload.source[1];
    lithium_config.source[2] = decoded_payload.source[2];
    lithium_config.source[3] = decoded_payload.source[3];
    lithium_config.source[4] = decoded_payload.source[4];
    lithium_config.source[5] = decoded_payload.source[5];
    lithium_config.destination[0] = decoded_payload.destination[0];
    lithium_config.destination[1] = decoded_payload.destination[1];
    lithium_config.destination[2] = decoded_payload.destination[2];
    lithium_config.destination[3] = decoded_payload.destination[3];
    lithium_config.destination[4] = decoded_payload.destination[4];
    lithium_config.destination[5] = decoded_payload.destination[5];
    lithium_config.tx_preamble = decoded_payload.tx_preamble_16 % 65536;
    lithium_config.tx_postamble = decoded_payload.tx_postamble_16 % 65536;
    lithium_config.function_config = decoded_payload.function_config_16 % 65536;
    lithium_config.function_config2 =
        decoded_payload.function_config2_16 % 65536;

    SetConfigurationCommand set_configuration_command(lithium_config);
    return Lithium::GetInstance()->DoCommand(&set_configuration_command);
}
