#ifndef SRC_TELECOMMS_LITHIUM_CONFIGURATION_H_
#define SRC_TELECOMMS_LITHIUM_CONFIGURATION_H_

#include <src/messages/message.h>
#include <src/messages/rebuildable_message.h>
#include <src/messages/rebuildable_message_field_iterator.h>
#include <src/util/data_types.h>
#include <string>
#include "lithium_function_config.h"

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

    LithiumConfiguration();
    explicit LithiumConfiguration(byte* serialised_message_buffer);
    SerialisedMessage SerialiseTo(byte* serial_buffer) const;
    uint16_t GetSerialisedSize() const;

    const std::string GetDestination() const;
    LithiumFunctionConfig* GetFunctionConfig();
    void SetFunctionConfig(LithiumFunctionConfig function_config);
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
    uint16_t tx_postamble;  // AX25 Mode Tx Postamble Byte Length (0x00 = 20
                            // flags)
    LithiumFunctionConfig
        function_config;  // Radio Configuration Discrete Behaviors

    void SerialiseFrom(byte* serial_buffer);
};

#endif  // SRC_TELECOMMS_LITHIUM_CONFIGURATION_H_
