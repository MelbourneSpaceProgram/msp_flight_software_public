#include <src/messages/rebuildable_message_field_iterator.h>
#include <src/messages/serialised_message_builder.h>
#include <src/telecomms/lithium_function_config.h>

// TODO(dingbenjamin): Find out from Gabi the required defaults
LithiumFunctionConfig::LithiumFunctionConfig()
    : external_event_function(kExternalEventOffLogicLow),
      config_1_function(kPin14OffLogicLow),
      config_2_function(kPin13OffLogicLow),
      telemetry_logging_rate(kTelemetryPacketLoggingRate1Hz),
      rx_crc_function_enable(false),
      tx_crc_function_enable(false),
      oa_function_enable(false),
      code_upload_function_enable(false),
      radio_reset_function_enable(false),
      factory_settings_flag(false),
      auto_frequency_control_enable(false) {}

LithiumFunctionConfig::LithiumFunctionConfig(byte* serial_buffer)
    : external_event_function(kExternalEventOffLogicLow),
      config_1_function(kPin14OffLogicLow),
      config_2_function(kPin13OffLogicLow),
      telemetry_logging_rate(kTelemetryPacketLoggingRate1Hz),
      rx_crc_function_enable(false),
      tx_crc_function_enable(false),
      oa_function_enable(false),
      code_upload_function_enable(false),
      radio_reset_function_enable(false),
      factory_settings_flag(false),
      auto_frequency_control_enable(false) {
    SerialiseFrom(serial_buffer);
}

SerialisedMessage LithiumFunctionConfig::SerialiseTo(
    byte* serial_buffer) const {
    uint32_t function_config_bits =
        static_cast<uint16_t>(external_event_function) |
        static_cast<uint16_t>(config_2_function) |
        static_cast<uint16_t>(config_1_function) |
        static_cast<uint16_t>(telemetry_logging_rate);
    function_config_bits |= rx_crc_function_enable ? kLithiumRxCrcMask : 0;
    function_config_bits |= tx_crc_function_enable ? kLithiumTxCrcMask : 0;
    function_config_bits |= oa_function_enable ? kLithiumOaMask : 0;
    function_config_bits |=
        code_upload_function_enable ? kLithiumCodeUploadMask : 0;
    function_config_bits |=
        radio_reset_function_enable ? kLithiumRadioResetMask : 0;
    function_config_bits |= auto_frequency_control_enable
                                ? (kLithiumAutoFrequencyControlMask << 8)
                                : 0;

    SerialisedMessageBuilder builder(serial_buffer, kFunctionConfigByteSize);
    builder.AddData<uint32_t>(function_config_bits);
    return builder.Build();
}

void LithiumFunctionConfig::SerialiseFrom(byte* serial_buffer) {
    RebuildableMessageFieldIterator field_iterator(serial_buffer,
                                                   kFunctionConfigByteSize);
    uint32_t function_config_bits;
    field_iterator.FetchData<uint32_t>(&function_config_bits);
    rx_crc_function_enable = function_config_bits & kLithiumRxCrcMask;
    tx_crc_function_enable = function_config_bits & kLithiumTxCrcMask;
    oa_function_enable = function_config_bits & kLithiumOaMask;
    code_upload_function_enable = function_config_bits & kLithiumCodeUploadMask;
    radio_reset_function_enable = function_config_bits & kLithiumRadioResetMask;
    auto_frequency_control_enable =
        (function_config_bits >> 8) & kLithiumAutoFrequencyControlMask;

    external_event_function = static_cast<LithiumExternalEventFunction>(
        function_config_bits & kLithiumExternalEventMask);
    config_1_function = static_cast<LithiumConfig1Function>(
        function_config_bits & kLithiumConfig1FunctionMask);
    config_2_function = static_cast<LithiumConfig2Function>(
        function_config_bits & kLithiumConfig2FunctionMask);
    telemetry_logging_rate = static_cast<LithiumTelemetryLoggingRate>(
        function_config_bits & kLithiumTelemetryLoggingRateMask);
}

uint16_t LithiumFunctionConfig::GetSerialisedSize() const {
    return kFunctionConfigByteSize;
}

bool LithiumFunctionConfig::IsAutoFrequencyControlEnable() const {
    return auto_frequency_control_enable;
}

void LithiumFunctionConfig::SetAutoFrequencyControlEnable(
    bool auto_frequency_control_enable) {
    this->auto_frequency_control_enable = auto_frequency_control_enable;
}

bool LithiumFunctionConfig::IsCodeUploadFunctionEnable() const {
    return code_upload_function_enable;
}

void LithiumFunctionConfig::SetCodeUploadFunctionEnable(
    bool code_upload_function_enable) {
    this->code_upload_function_enable = code_upload_function_enable;
}

LithiumFunctionConfig::LithiumConfig1Function
LithiumFunctionConfig::GetConfig1Function() const {
    return config_1_function;
}

void LithiumFunctionConfig::SetConfig1Function(
    LithiumFunctionConfig::LithiumConfig1Function config_1_function) {
    this->config_1_function = config_1_function;
}

LithiumFunctionConfig::LithiumConfig2Function
LithiumFunctionConfig::GetConfig2Function() const {
    return config_2_function;
}

void LithiumFunctionConfig::SetConfig2Function(
    LithiumConfig2Function config_2_function) {
    this->config_2_function = config_2_function;
}

LithiumFunctionConfig::LithiumExternalEventFunction
LithiumFunctionConfig::GetExternalEventFunction() const {
    return external_event_function;
}

void LithiumFunctionConfig::SetExternalEventFunction(
    LithiumExternalEventFunction external_event_function) {
    this->external_event_function = external_event_function;
}

bool LithiumFunctionConfig::IsFactorySettingsFlag() const {
    return factory_settings_flag;
}

bool LithiumFunctionConfig::IsOaFunctionEnable() const {
    return oa_function_enable;
}

void LithiumFunctionConfig::SetOaFunctionEnable(bool oa_function_enable) {
    this->oa_function_enable = oa_function_enable;
}

bool LithiumFunctionConfig::IsRadioResetFunctionEnable() const {
    return radio_reset_function_enable;
}

void LithiumFunctionConfig::SetRadioResetFunctionEnable(
    bool radio_reset_function_enable) {
    this->radio_reset_function_enable = radio_reset_function_enable;
}

bool LithiumFunctionConfig::IsRxCrcFunctionEnable() const {
    return rx_crc_function_enable;
}

void LithiumFunctionConfig::SetRxCrcFunctionEnable(
    bool rx_crc_function_enable) {
    this->rx_crc_function_enable = rx_crc_function_enable;
}

LithiumFunctionConfig::LithiumTelemetryLoggingRate
LithiumFunctionConfig::GetTelemetryLoggingRate() const {
    return telemetry_logging_rate;
}

void LithiumFunctionConfig::SetTelemetryLoggingRate(
    LithiumTelemetryLoggingRate telemetry_logging_rate) {
    this->telemetry_logging_rate = telemetry_logging_rate;
}

bool LithiumFunctionConfig::IsTxCrcFunctionEnable() const {
    return tx_crc_function_enable;
}

void LithiumFunctionConfig::SetTxCrcFunctionEnable(
    bool tx_crc_function_enable) {
    this->tx_crc_function_enable = tx_crc_function_enable;
}
