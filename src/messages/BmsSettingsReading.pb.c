/* Automatically generated nanopb constant definitions */
/* Generated by nanopb-0.3.9.1 at Wed Sep 26 21:59:31 2018. */

#include "BmsSettingsReading.pb.h"

/* @@protoc_insertion_point(includes) */
#if PB_PROTO_HEADER_VERSION != 30
#error Regenerate this file with the current version of nanopb generator.
#endif

const uint32_t BmsSettingsReading_charger_config_default = 9999u;
const uint32_t BmsSettingsReading_v_charge_setting_default = 9999u;
const uint32_t BmsSettingsReading_i_charge_target_default = 9999u;
const uint32_t BmsSettingsReading_config_bits_default = 9999u;
const uint32_t BmsSettingsReading_v_in_uvcl_setting_default = 9999u;
const bool BmsSettingsReading_telemetry_valid_default = false;
const uint64_t BmsSettingsReading_timestamp_ms_default = 0ull;


const pb_field_t BmsSettingsReading_fields[8] = {
    PB_FIELD(  1, UINT32  , REQUIRED, STATIC  , FIRST, BmsSettingsReading, charger_config, charger_config, &BmsSettingsReading_charger_config_default),
    PB_FIELD(  2, UINT32  , REQUIRED, STATIC  , OTHER, BmsSettingsReading, v_charge_setting, charger_config, &BmsSettingsReading_v_charge_setting_default),
    PB_FIELD(  3, UINT32  , REQUIRED, STATIC  , OTHER, BmsSettingsReading, i_charge_target, v_charge_setting, &BmsSettingsReading_i_charge_target_default),
    PB_FIELD(  4, UINT32  , REQUIRED, STATIC  , OTHER, BmsSettingsReading, config_bits, i_charge_target, &BmsSettingsReading_config_bits_default),
    PB_FIELD(  5, UINT32  , REQUIRED, STATIC  , OTHER, BmsSettingsReading, v_in_uvcl_setting, config_bits, &BmsSettingsReading_v_in_uvcl_setting_default),
    PB_FIELD(  6, BOOL    , REQUIRED, STATIC  , OTHER, BmsSettingsReading, telemetry_valid, v_in_uvcl_setting, &BmsSettingsReading_telemetry_valid_default),
    PB_FIELD(  7, FIXED64 , REQUIRED, STATIC  , OTHER, BmsSettingsReading, timestamp_ms, telemetry_valid, &BmsSettingsReading_timestamp_ms_default),
    PB_LAST_FIELD
};


/* @@protoc_insertion_point(eof) */