#ifndef SRC_TELECOMMS_LITHIUM_CONFIGURATION_H_
#define SRC_TELECOMMS_LITHIUM_CONFIGURATION_H_

#include <src/messages/message.h>
#include <src/messages/rebuildable_message.h>
#include <src/messages/rebuildable_message_field_iterator.h>
#include <src/util/data_types.h>
#include <string>

class LithiumConfiguration : public RebuildableMessage {
   public:
    enum LithiumBaudRate {
        kBaud9600 = 0x00,
        kBaud19200 = 0x01,
        kBaud38400 = 0x02,
        kBaud76800 = 0x03,
        kBaud115200 = 0x04
    };

    enum LithiumRfBaudRate {
        kRfBaud1200 = 0x00,
        kRfBaud9600 = 0x01,
        kRfBaud19200 = 0x02,
        kRfBaud38400 = 0x03
    };

    enum LithiumRfModulation { kGfsk = 0x00, kAfsk = 0x01, kBpsk = 0x02 };

    enum LithiumOverAirCommands {
        kTelemetryDumpCommand = 0x30,
        kPingReturnCommand = 0x31,
        kCodeUploadCOmmand = 0x32,
        kRadioResetCommand = 0x33,
        kPinToggleCommand = 0x34
    };

    // Refer to Radio Interface Manual for full descriptions
    // of function config enumerations below

    // Pin 12
    enum LithiumExternalEventFunctions {
        kExternalEventOffLogicLow = 0b0000000000000000,
        k2500MsToggle = 0b0000000000000001,
        kTxPacket12MsToggle = 0b0000000000000010,
        kRxPacket1250UsToggle = 0b0000000000000011
    };

    // Pin 13
    enum LithiumConfig2Functions {
        kPin13OffLogicLow = 0b0000000000000000,
        kTxRxSwitch = 0b0000000000000100,
        k2500kHzWdt = 0b000000000001000,
        kRxPacketToggle = 0b0000000000001100
    };

    // Pin 14
    enum LithiumConfig1Functions {
        kPin14OffLogicLow = 0b0000000000000000,
        kDioOaEnable = 0b0000000000010000,
        kDioOaPatternA = 0b0000000000000000,
        kDioOaPatternB = 0b0000000000100000
    };

    enum LithiumCrcFunctions {
        kRxCrcDisable = 0b0000000000000000,
        kRxCrcEnable = 0b0000000001000000,
        kTxCrcDisable = 0b0000000000000000,  // TBD by Astrodev
        kTxCrcEnable = 0b0000000010000000    // TBD by Astrodev
    };

    enum LithiumTelemetryFunctions {
        kTelemetryPacketLoggingDisable = 0b0000000000000000,
        kTelemetryPacketLoggingEnable = 0b0000000100000000,
        kTelemetryPacketLoggingRate100mHz = 0b0000000000000000,
        kTelemetryPacketLoggingRate1Hz = 0b0000001000000000,
        kTelemetryPacketLoggingRate2Hz = 0b0000010000000000,
        kTelemetryPacketLoggingRate4Hz = 0b0000011000000000,
        kTelemetryPacketDumpDisable = 0b0000000000000000,
        kTelemetryPacketDumpEnable = 0b0000100000000000
    };

    enum LithiumBeaconFunctions {
        kOaCommandsDisable = 0b0000000000000000,
        kOaCOmmandsEnable = 0b0001000000000000,
        kCodeUploadDisable = 0b0000000000000000,
        kCodeUploadEnable = 0b0010000000000000,
        kRadioResetDisable = 0b0000000000000000,
        kRadioResetEnable = 0b0100000000000000
    };

    enum LithiumFactoryDefaultsRestored {
        kFactorySettingsRestoreCompleteFlag = 0b1000000000000000
    };

    LithiumConfiguration();
    explicit LithiumConfiguration(byte* serialised_message_buffer);
    SerialisedMessage SerialiseTo(byte* serial_buffer);
    uint16_t GetSerialisedSize() const;

    const std::string GetDestination() const;
    uint16_t GetFunctionConfig() const;
    void SetFunctionConfig(uint16_t function_config);
    uint16_t GetFunctionConfig2() const;
    void SetFunctionConfig2(uint16_t function_config_2);
    LithiumBaudRate GetInterfaceBaudRate() const;
    void SetInterfaceBaudRate(LithiumBaudRate interface_baud_rate);
    uint32_t GetRxFreq() const;
    void SetRxFreq(uint32_t rx_freq);
    LithiumRfModulation GetRxModulation() const;
    void SetRxModulation(LithiumRfModulation rx_modulation);
    LithiumRfBaudRate GetRxRfBaudRate() const;
    void SetRxRfBaudRate(LithiumRfBaudRate rx_rf_baud_rate);
    const std::string GetSource() const;
    uint32_t GetTxFreq() const;
    void SetTxFreq(uint32_t tx_freq);
    LithiumRfModulation GetTxModulation() const;
    void SetTxModulation(LithiumRfModulation tx_modulation);
    uint16_t GetTxPostamble() const;
    void SetTxPostamble(uint16_t tx_postamble);
    uint8_t GetTxPowerAmpLevel() const;
    void SetTxPowerAmpLevel(uint8_t tx_power_amp_level);
    uint16_t GetTxPreamble() const;
    void SetTxPreamble(uint16_t tx_preamble);
    LithiumRfBaudRate GetTxRfBaudRate() const;
    void SetTxRfBaudRate(LithiumRfBaudRate tx_rf_baud_rate);

   private:
    static const uint16_t kSerialisedSize = 34;
    static const uint8_t kCallsignLength = 6;
    static const LithiumBaudRate kDefaultInterfaceBaudRate = kBaud9600;
    static const uint8_t kDefaultTxPowerAmpLevel = 150;
    static const LithiumRfBaudRate kDefaultRxRfBaudRate = kRfBaud9600;
    static const LithiumRfBaudRate kDefaultTxRfBaudRate = kRfBaud9600;
    static const LithiumRfModulation kDefaultRxModulation = kGfsk;
    static const LithiumRfModulation kDefaultTxModulation = kGfsk;
    static const uint32_t kDefaultRxFreq = 45000000;
    static const uint32_t kDefaultTxFreq = 45000000;
    static const std::string kDefaultSource;       // 6 Chars
    static const std::string kDefaultDestination;  // 6 Chars
    static const uint16_t kDefaultTxPreamble =
        0x00;  // TODO(dingbenjamin): Find default
    static const uint16_t kDefaultTxPostamble =
        0x00;  // TODO(dingbenjamin): Find default
    static const uint16_t kDefaultFunctionConfig =
        0x00;  // TODO(dingbenjamin): Find default value
    static const uint16_t kDefaultFunctionConfig2 =
        0x00;  // TODO(dingbenjamin): Find default value

    LithiumBaudRate interface_baud_rate;
    uint8_t tx_power_amp_level;
    LithiumRfBaudRate rx_rf_baud_rate;
    LithiumRfBaudRate tx_rf_baud_rate;
    LithiumRfModulation rx_modulation;
    LithiumRfModulation tx_modulation;
    uint32_t rx_freq;
    uint32_t tx_freq;
    std::string source;  // AX25 Mode Source Call Sign (hardware default NOCALL)
    std::string
        destination;  // AX25 Mode Destination Call Sign (harware default CQ)
    uint16_t
        tx_preamble;  // AX25 Mode Tx Preamble Byte Length (0x00 = 20 flags)
    uint16_t tx_postamble;      // AX25 Mode Tx Postamble Byte Length (0x00 = 20
                                // flags)
    uint16_t function_config;   // Radio Configuration Discrete Behaviors
    uint16_t function_config2;  // Radio Configuration Discrete Behaviors #2

    void SerialiseFrom(byte* serial_buffer);
};

#endif  // SRC_TELECOMMS_LITHIUM_CONFIGURATION_H_
