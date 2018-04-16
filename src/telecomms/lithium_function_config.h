#ifndef SRC_TELECOMMS_LITHIUM_FUNCTION_CONFIG_H_
#define SRC_TELECOMMS_LITHIUM_FUNCTION_CONFIG_H_

#include <src/messages/rebuildable_message.h>

class LithiumFunctionConfig : public RebuildableMessage {
   public:
    // Refer to Radio Interface Manual for full descriptions
    // of function config enumerations below

    // Pin 12
    enum LithiumExternalEventFunction {
        kExternalEventOffLogicLow = 0b0000000000000000,
        k2500MsToggle = 0b0000000000000001,
        kTxPacket12MsToggle = 0b0000000000000010,
        kRxPacket1250UsToggle = 0b0000000000000011
    };

    // Pin 13
    enum LithiumConfig2Function {
        kPin13OffLogicLow = 0b0000000000000000,
        kTxRxSwitch = 0b0000000000000100,
        k2500kHzWdt = 0b000000000001000,
        kRxPacketToggle = 0b0000000000001100
    };

    // Pin 14
    enum LithiumConfig1Function {
        kPin14OffLogicLow = 0b0000000000000000,
        kDioOaEnable = 0b0000000000010000,
        kDioOaPatternA = 0b0000000000000000,
        kDioOaPatternB = 0b0000000000100000
    };

    enum LithiumTelemetryLoggingRate {
        kTelemetryPacketLoggingRate100mHz = 0b0000000000000000,
        kTelemetryPacketLoggingRate1Hz = 0b0000001000000000,
        kTelemetryPacketLoggingRate2Hz = 0b0000010000000000,
        kTelemetryPacketLoggingRate4Hz = 0b0000011000000000,
    };

    static const uint16_t kFunctionConfigByteSize = 4;
    static const uint16_t kLithiumExternalEventMask = 0b0000000000000011;
    static const uint16_t kLithiumConfig2FunctionMask = 0b0000000000001100;
    static const uint16_t kLithiumConfig1FunctionMask = 0b0000000000110000;
    static const uint16_t kLithiumTelemetryLoggingRateMask = 0b0000011000000000;
    static const uint16_t kLithiumAutoFrequencyControlMask = 0b0000000000000001;
    static const uint16_t kLithiumRadioResetMask = 0b0100000000000000;
    static const uint16_t kLithiumCodeUploadMask = 0b0010000000000000;
    static const uint16_t kLithiumOaMask = 0b0001000000000000;
    static const uint16_t kLithiumTelemetryPacketDumpMask = 0b0000100000000000;
    static const uint16_t kLithiumRxCrcMask = 0b0000000001000000;
    static const uint16_t kLithiumFactoryDefaultsRestoredMask =
        0b1000000000000000;
    static const uint16_t kLithiumTxCrcMask =
        0b0000000010000000;  // TBD by Astrodev
    static const uint16_t kLithiumTelemetryPacketLoggingMask =
        0b0000000100000000;
    LithiumFunctionConfig();
    explicit LithiumFunctionConfig(byte* serial_buffer);
    SerialisedMessage SerialiseTo(byte* serial_buffer) const;
    uint16_t GetSerialisedSize() const;

    bool IsAutoFrequencyControlEnable() const;
    void SetAutoFrequencyControlEnable(bool auto_frequency_control_enable);
    bool IsCodeUploadFunctionEnable() const;
    void SetCodeUploadFunctionEnable(bool code_upload_function_enable);
    LithiumConfig1Function GetConfig1Function() const;
    void SetConfig1Function(LithiumConfig1Function config_1_function);
    LithiumConfig2Function GetConfig2Function() const;
    void SetConfig2Function(LithiumConfig2Function config_2_function);
    LithiumExternalEventFunction GetExternalEventFunction() const;
    void SetExternalEventFunction(
        LithiumExternalEventFunction external_event_function);
    bool IsFactorySettingsFlag() const;
    bool IsOaFunctionEnable() const;
    void SetOaFunctionEnable(bool oa_function_enable);
    bool IsRadioResetFunctionEnable() const;
    void SetRadioResetFunctionEnable(bool radio_reset_function_enable);
    bool IsRxCrcFunctionEnable() const;
    void SetRxCrcFunctionEnable(bool rx_crc_function_enable);
    LithiumTelemetryLoggingRate GetTelemetryLoggingRate() const;
    void SetTelemetryLoggingRate(
        LithiumTelemetryLoggingRate telemetry_logging_rate);
    bool IsTxCrcFunctionEnable() const;
    void SetTxCrcFunctionEnable(bool tx_crc_function_enable);

   private:
    void SerialiseFrom(byte* serial_buffer);
    LithiumExternalEventFunction external_event_function;
    LithiumConfig1Function config_1_function;
    LithiumConfig2Function config_2_function;
    LithiumTelemetryLoggingRate telemetry_logging_rate;
    bool rx_crc_function_enable;
    bool tx_crc_function_enable;
    bool oa_function_enable;
    bool code_upload_function_enable;
    bool radio_reset_function_enable;
    bool factory_settings_flag;
    bool auto_frequency_control_enable;
};

#endif  // SRC_TELECOMMS_LITHIUM_FUNCTION_CONFIG_H_
