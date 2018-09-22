#ifndef SRC_TELECOMMS_LITHIUM_CONFIGURATION_H_
#define SRC_TELECOMMS_LITHIUM_CONFIGURATION_H_

// See Lithium Radio Interface Datasheet
typedef struct LithiumConfiguration {
    uint8_t interface_baud_rate;
    uint8_t tx_power_amp_level;
    uint8_t rx_rf_baud_rate;
    uint8_t tx_rf_baud_rate;
    uint8_t rx_modulation;
    uint8_t tx_modulation;
    uint32_t rx_freq;
    uint32_t tx_freq;
    byte source[6];
    byte destination[6];
    uint16_t tx_preamble;
    uint16_t tx_postamble;
    uint16_t function_config;
    uint16_t function_config2;
} LithiumConfig;

#endif  // SRC_TELECOMMS_LITHIUM_CONFIGURATION_H_
