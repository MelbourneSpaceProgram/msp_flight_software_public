#include <src/board/uart/uart.h>
#include <src/messages/serialised_message_builder.h>
#include <src/telecomms/lithium_configuration.h>

const std::string LithiumConfiguration::kDefaultSource = "ACRUX1";
const std::string LithiumConfiguration::kDefaultDestination = "VK3EXO";

LithiumConfiguration::LithiumConfiguration()
    : interface_baud_rate(kDefaultInterfaceBaudRate),
      tx_power_amp_level(kDefaultTxPowerAmpLevel),
      rx_rf_baud_rate(kDefaultRxRfBaudRate),
      tx_rf_baud_rate(kDefaultTxRfBaudRate),
      rx_modulation(kDefaultRxModulation),
      tx_modulation(kDefaultTxModulation),
      rx_freq(kDefaultRxFreq),
      tx_freq(kDefaultTxFreq),
      source(kDefaultSource),
      destination(kDefaultDestination),
      tx_preamble(kDefaultTxPreamble),
      tx_postamble(kDefaultTxPostamble),
      function_config() {}

LithiumConfiguration::LithiumConfiguration(byte* serialised_message_buffer)
    : interface_baud_rate(kDefaultInterfaceBaudRate),
      tx_power_amp_level(kDefaultTxPowerAmpLevel),
      rx_rf_baud_rate(kDefaultRxRfBaudRate),
      tx_rf_baud_rate(kDefaultTxRfBaudRate),
      rx_modulation(kDefaultRxModulation),
      tx_modulation(kDefaultTxModulation),
      rx_freq(kDefaultRxFreq),
      tx_freq(kDefaultTxFreq),
      source(kDefaultSource),
      destination(kDefaultDestination),
      tx_preamble(kDefaultTxPreamble),
      tx_postamble(kDefaultTxPostamble),
      function_config() {
    SerialiseFrom(serialised_message_buffer);
}

SerialisedMessage LithiumConfiguration::SerialiseTo(byte* serial_buffer) const {
    SerialisedMessageBuilder builder(serial_buffer, GetSerialisedSize());

    return builder.AddData<uint8_t>(interface_baud_rate)
        .AddData<uint8_t>(tx_power_amp_level)
        .AddData<uint8_t>(rx_rf_baud_rate)
        .AddData<uint8_t>(tx_rf_baud_rate)
        .AddData<uint8_t>(rx_modulation)
        .AddData<uint8_t>(tx_modulation)
        .AddData<uint32_t>(rx_freq)
        .AddData<uint32_t>(tx_freq)
        .AddData<char>(source.at(0))
        .AddData<char>(source.at(1))
        .AddData<char>(source.at(2))
        .AddData<char>(source.at(3))
        .AddData<char>(source.at(4))
        .AddData<char>(source.at(5))
        .AddData<char>(destination.at(0))
        .AddData<char>(destination.at(1))
        .AddData<char>(destination.at(2))
        .AddData<char>(destination.at(3))
        .AddData<char>(destination.at(4))
        .AddData<char>(destination.at(5))
        .AddData<uint16_t>(tx_preamble)
        .AddData<uint16_t>(tx_postamble)
        .AddMessage(&function_config)
        .Build();
}

void LithiumConfiguration::SerialiseFrom(byte* serial_buffer) {
    uint8_t tx_modulation_temp, rx_modulation_temp, tx_rf_baud_rate_temp,
        rx_rf_baud_rate_temp, interface_baud_rate_temp;
    byte function_config_bytes[4];
    char destination_temp[6];
    char source_temp[6];

    RebuildableMessageFieldIterator config_iterator(serial_buffer,
                                                    GetSerialisedSize());
    config_iterator.FetchData<byte>(&function_config_bytes[3]);
    config_iterator.FetchData<byte>(&function_config_bytes[2]);
    config_iterator.FetchData<byte>(&function_config_bytes[1]);
    config_iterator.FetchData<byte>(&function_config_bytes[0]);
    config_iterator.FetchData<uint16_t>(&tx_postamble);
    config_iterator.FetchData<uint16_t>(&tx_preamble);
    for (int char_index = kCallsignLength - 1; char_index >= 0; char_index--) {
        config_iterator.FetchData<char>(&destination_temp[char_index]);
    }
    for (int char_index = kCallsignLength - 1; char_index >= 0; char_index--) {
        config_iterator.FetchData<char>(&source_temp[char_index]);
    }
    config_iterator.FetchData<uint32_t>(&tx_freq);
    config_iterator.FetchData<uint32_t>(&rx_freq);
    config_iterator.FetchData<uint8_t>(&tx_modulation_temp);
    config_iterator.FetchData<uint8_t>(&rx_modulation_temp);
    config_iterator.FetchData<uint8_t>(&tx_rf_baud_rate_temp);
    config_iterator.FetchData<uint8_t>(&rx_rf_baud_rate_temp);
    config_iterator.FetchData<uint8_t>(&tx_power_amp_level);
    config_iterator.FetchData<uint8_t>(&interface_baud_rate_temp);

    tx_modulation = static_cast<LithiumRfModulation>(tx_modulation_temp);
    rx_modulation = static_cast<LithiumRfModulation>(rx_modulation_temp);
    tx_rf_baud_rate = static_cast<LithiumRfBaudRate>(tx_rf_baud_rate_temp);
    rx_rf_baud_rate = static_cast<LithiumRfBaudRate>(rx_rf_baud_rate_temp);
    interface_baud_rate =
        static_cast<LithiumBaudRate>(interface_baud_rate_temp);
    source = std::string(source_temp, kCallsignLength);
    destination = std::string(destination_temp, kCallsignLength);
    function_config = LithiumFunctionConfig(function_config_bytes);
}

uint16_t LithiumConfiguration::GetSerialisedSize() const {
    return LithiumConfiguration::kSerialisedSize;
}

const std::string LithiumConfiguration::GetDestination() const {
    return destination;
}

LithiumFunctionConfig* LithiumConfiguration::GetFunctionConfig() {
    return &function_config;
}

void LithiumConfiguration::SetFunctionConfig(
    LithiumFunctionConfig function_config) {
    this->function_config = function_config;
}

LithiumConfiguration::LithiumBaudRate
LithiumConfiguration::GetInterfaceBaudRate() const {
    return interface_baud_rate;
}

void LithiumConfiguration::SetInterfaceBaudRate(
    LithiumConfiguration::LithiumBaudRate interface_baud_rate) {
    this->interface_baud_rate = interface_baud_rate;
}

uint32_t LithiumConfiguration::GetRxFreq() const { return rx_freq; }

void LithiumConfiguration::SetRxFreq(uint32_t rx_freq) {
    this->rx_freq = rx_freq;
}

LithiumConfiguration::LithiumRfModulation
LithiumConfiguration::GetRxModulation() const {
    return rx_modulation;
}

void LithiumConfiguration::SetRxModulation(
    LithiumConfiguration::LithiumRfModulation rx_modulation) {
    this->rx_modulation = rx_modulation;
}

LithiumConfiguration::LithiumRfBaudRate LithiumConfiguration::GetRxRfBaudRate()
    const {
    return rx_rf_baud_rate;
}

void LithiumConfiguration::SetRxRfBaudRate(
    LithiumConfiguration::LithiumRfBaudRate rx_rf_baud_rate) {
    this->rx_rf_baud_rate = rx_rf_baud_rate;
}

const std::string LithiumConfiguration::GetSource() const { return source; }

uint32_t LithiumConfiguration::GetTxFreq() const { return tx_freq; }

void LithiumConfiguration::SetTxFreq(uint32_t tx_freq) {
    this->tx_freq = tx_freq;
}

LithiumConfiguration::LithiumRfModulation
LithiumConfiguration::GetTxModulation() const {
    return tx_modulation;
}

void LithiumConfiguration::SetTxModulation(
    LithiumConfiguration::LithiumRfModulation tx_modulation) {
    this->tx_modulation = tx_modulation;
}

uint16_t LithiumConfiguration::GetTxPostamble() const { return tx_postamble; }

void LithiumConfiguration::SetTxPostamble(uint16_t tx_postamble) {
    this->tx_postamble = tx_postamble;
}

uint8_t LithiumConfiguration::GetTxPowerAmpLevel() const {
    return tx_power_amp_level;
}

void LithiumConfiguration::SetTxPowerAmpLevel(uint8_t tx_power_amp_level) {
    this->tx_power_amp_level = tx_power_amp_level;
}

uint16_t LithiumConfiguration::GetTxPreamble() const { return tx_preamble; }

void LithiumConfiguration::SetTxPreamble(uint16_t tx_preamble) {
    this->tx_preamble = tx_preamble;
}

LithiumConfiguration::LithiumRfBaudRate LithiumConfiguration::GetTxRfBaudRate()
    const {
    return tx_rf_baud_rate;
}

void LithiumConfiguration::SetTxRfBaudRate(
    LithiumConfiguration::LithiumRfBaudRate tx_rf_baud_rate) {
    this->tx_rf_baud_rate = tx_rf_baud_rate;
}
