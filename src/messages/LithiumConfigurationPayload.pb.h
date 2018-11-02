/* Automatically generated nanopb header */
/* Generated by nanopb-0.3.9.1 at Wed Oct 31 15:27:18 2018. */

#ifndef PB_LITHIUMCONFIGURATIONPAYLOAD_PB_H_INCLUDED
#define PB_LITHIUMCONFIGURATIONPAYLOAD_PB_H_INCLUDED
#include <pb.h>

/* @@protoc_insertion_point(includes) */
#if PB_PROTO_HEADER_VERSION != 30
#error Regenerate this file with the current version of nanopb generator.
#endif

#ifdef __cplusplus
extern "C" {
#endif

/* Struct definitions */
typedef struct _LithiumConfigurationPayload {
    uint32_t interface_baud_rate_8;
    uint32_t tx_power_amp_level_8;
    uint32_t rx_rf_baud_rate_8;
    uint32_t tx_rf_baud_rate_8;
    uint32_t rx_modulation_8;
    uint32_t tx_modulation_8;
    uint32_t rx_freq_32;
    uint32_t tx_freq_32;
    pb_byte_t source[6];
    pb_byte_t destination[6];
    uint32_t tx_preamble_16;
    uint32_t tx_postamble_16;
    uint32_t function_config_16;
    uint32_t function_config2_16;
/* @@protoc_insertion_point(struct:LithiumConfigurationPayload) */
} LithiumConfigurationPayload;

/* Default values for struct fields */
extern const uint32_t LithiumConfigurationPayload_interface_baud_rate_8_default;
extern const uint32_t LithiumConfigurationPayload_tx_power_amp_level_8_default;
extern const uint32_t LithiumConfigurationPayload_rx_rf_baud_rate_8_default;
extern const uint32_t LithiumConfigurationPayload_tx_rf_baud_rate_8_default;
extern const uint32_t LithiumConfigurationPayload_rx_modulation_8_default;
extern const uint32_t LithiumConfigurationPayload_tx_modulation_8_default;
extern const uint32_t LithiumConfigurationPayload_rx_freq_32_default;
extern const uint32_t LithiumConfigurationPayload_tx_freq_32_default;
extern const pb_byte_t LithiumConfigurationPayload_source_default[6];
extern const pb_byte_t LithiumConfigurationPayload_destination_default[6];
extern const uint32_t LithiumConfigurationPayload_tx_preamble_16_default;
extern const uint32_t LithiumConfigurationPayload_tx_postamble_16_default;
extern const uint32_t LithiumConfigurationPayload_function_config_16_default;
extern const uint32_t LithiumConfigurationPayload_function_config2_16_default;

/* Initializer values for message structs */
#define LithiumConfigurationPayload_init_default {0u, 140u, 1u, 1u, 0u, 0u, 437425u, 437425u, {0x56,0x4b,0x33,0x45,0x58,0x4f}, {0x43,0x51,0x20,0x20,0x20,0x20}, 16u, 0u, 28673u, 0u}
#define LithiumConfigurationPayload_init_zero    {0, 0, 0, 0, 0, 0, 0, 0, {0}, {0}, 0, 0, 0, 0}

/* Field tags (for use in manual encoding/decoding) */
#define LithiumConfigurationPayload_interface_baud_rate_8_tag 1
#define LithiumConfigurationPayload_tx_power_amp_level_8_tag 2
#define LithiumConfigurationPayload_rx_rf_baud_rate_8_tag 3
#define LithiumConfigurationPayload_tx_rf_baud_rate_8_tag 4
#define LithiumConfigurationPayload_rx_modulation_8_tag 5
#define LithiumConfigurationPayload_tx_modulation_8_tag 6
#define LithiumConfigurationPayload_rx_freq_32_tag 7
#define LithiumConfigurationPayload_tx_freq_32_tag 8
#define LithiumConfigurationPayload_source_tag   10
#define LithiumConfigurationPayload_destination_tag 11
#define LithiumConfigurationPayload_tx_preamble_16_tag 12
#define LithiumConfigurationPayload_tx_postamble_16_tag 13
#define LithiumConfigurationPayload_function_config_16_tag 14
#define LithiumConfigurationPayload_function_config2_16_tag 15

/* Struct field encoding specification for nanopb */
extern const pb_field_t LithiumConfigurationPayload_fields[15];

/* Maximum encoded size of messages (where known) */
#define LithiumConfigurationPayload_size         88

/* Message IDs (where set with "msgid" option) */
#ifdef PB_MSGID

#define LITHIUMCONFIGURATIONPAYLOAD_MESSAGES \


#endif

#ifdef __cplusplus
} /* extern "C" */
#endif
/* @@protoc_insertion_point(eof) */

#endif