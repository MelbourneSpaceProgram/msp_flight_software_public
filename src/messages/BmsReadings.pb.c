/* Automatically generated nanopb constant definitions */
/* Generated by nanopb-0.3.9.1 at Sat Aug 18 23:19:37 2018. */

#include "BmsReadings.pb.h"

/* @@protoc_insertion_point(includes) */
#if PB_PROTO_HEADER_VERSION != 30
#error Regenerate this file with the current version of nanopb generator.
#endif



const pb_field_t BmsReadings_fields[24] = {
    PB_FIELD(  1, DOUBLE  , REQUIRED, STATIC  , FIRST, BmsReadings, battery_voltage, battery_voltage, 0),
    PB_FIELD(  2, DOUBLE  , REQUIRED, STATIC  , OTHER, BmsReadings, battery_current, battery_voltage, 0),
    PB_FIELD(  3, DOUBLE  , REQUIRED, STATIC  , OTHER, BmsReadings, system_voltage, battery_current, 0),
    PB_FIELD(  4, DOUBLE  , REQUIRED, STATIC  , OTHER, BmsReadings, input_voltage, system_voltage, 0),
    PB_FIELD(  5, DOUBLE  , REQUIRED, STATIC  , OTHER, BmsReadings, input_current, input_voltage, 0),
    PB_FIELD(  6, DOUBLE  , REQUIRED, STATIC  , OTHER, BmsReadings, die_temperature, input_current, 0),
    PB_FIELD(  7, DOUBLE  , REQUIRED, STATIC  , OTHER, BmsReadings, battery_temperature, die_temperature, 0),
    PB_FIELD(  8, UINT32  , REQUIRED, STATIC  , OTHER, BmsReadings, jeita_region, battery_temperature, 0),
    PB_FIELD(  9, UINT32  , REQUIRED, STATIC  , OTHER, BmsReadings, system_status, jeita_region, 0),
    PB_FIELD( 10, UINT32  , REQUIRED, STATIC  , OTHER, BmsReadings, charger_state, system_status, 0),
    PB_FIELD( 11, UINT32  , REQUIRED, STATIC  , OTHER, BmsReadings, charge_status, charger_state, 0),
    PB_FIELD( 12, DOUBLE  , REQUIRED, STATIC  , OTHER, BmsReadings, recharge_threshold, charge_status, 0),
    PB_FIELD( 13, UINT32  , REQUIRED, STATIC  , OTHER, BmsReadings, charger_config, recharge_threshold, 0),
    PB_FIELD( 14, UINT32  , REQUIRED, STATIC  , OTHER, BmsReadings, c_over_x_threshold, charger_config, 0),
    PB_FIELD( 15, UINT32  , REQUIRED, STATIC  , OTHER, BmsReadings, v_charge_setting, c_over_x_threshold, 0),
    PB_FIELD( 16, UINT32  , REQUIRED, STATIC  , OTHER, BmsReadings, i_charge_target, v_charge_setting, 0),
    PB_FIELD( 17, UINT32  , REQUIRED, STATIC  , OTHER, BmsReadings, config_bits, i_charge_target, 0),
    PB_FIELD( 18, UINT32  , REQUIRED, STATIC  , OTHER, BmsReadings, q_count, config_bits, 0),
    PB_FIELD( 19, UINT32  , REQUIRED, STATIC  , OTHER, BmsReadings, q_count_prescale_factor, q_count, 0),
    PB_FIELD( 20, UINT32  , REQUIRED, STATIC  , OTHER, BmsReadings, v_in_uvcl_setting, q_count_prescale_factor, 0),
    PB_FIELD( 21, DOUBLE  , REQUIRED, STATIC  , OTHER, BmsReadings, v_charge_dac, v_in_uvcl_setting, 0),
    PB_FIELD( 22, DOUBLE  , REQUIRED, STATIC  , OTHER, BmsReadings, i_charge_dac, v_charge_dac, 0),
    PB_FIELD( 23, BOOL    , REQUIRED, STATIC  , OTHER, BmsReadings, telemetry_valid, i_charge_dac, 0),
    PB_LAST_FIELD
};


/* On some platforms (such as AVR), double is really float.
 * These are not directly supported by nanopb, but see example_avr_double.
 * To get rid of this error, remove any double fields from your .proto.
 */
PB_STATIC_ASSERT(sizeof(double) == 8, DOUBLE_MUST_BE_8_BYTES)

/* @@protoc_insertion_point(eof) */
