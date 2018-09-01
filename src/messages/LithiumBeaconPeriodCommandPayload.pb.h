/* Automatically generated nanopb header */
/* Generated by nanopb-0.3.9.1 at Sat Sep 01 13:53:57 2018. */

#ifndef PB_LITHIUMBEACONPERIODCOMMANDPAYLOAD_PB_H_INCLUDED
#define PB_LITHIUMBEACONPERIODCOMMANDPAYLOAD_PB_H_INCLUDED
#include <pb.h>

/* @@protoc_insertion_point(includes) */
#if PB_PROTO_HEADER_VERSION != 30
#error Regenerate this file with the current version of nanopb generator.
#endif

#ifdef __cplusplus
extern "C" {
#endif

/* Struct definitions */
typedef struct _LithiumBeaconPeriodCommandPayload {
    uint32_t beacon_period_ms;
/* @@protoc_insertion_point(struct:LithiumBeaconPeriodCommandPayload) */
} LithiumBeaconPeriodCommandPayload;

/* Default values for struct fields */

/* Initializer values for message structs */
#define LithiumBeaconPeriodCommandPayload_init_default {0}
#define LithiumBeaconPeriodCommandPayload_init_zero {0}

/* Field tags (for use in manual encoding/decoding) */
#define LithiumBeaconPeriodCommandPayload_beacon_period_ms_tag 1

/* Struct field encoding specification for nanopb */
extern const pb_field_t LithiumBeaconPeriodCommandPayload_fields[2];

/* Maximum encoded size of messages (where known) */
#define LithiumBeaconPeriodCommandPayload_size   6

/* Message IDs (where set with "msgid" option) */
#ifdef PB_MSGID

#define LITHIUMBEACONPERIODCOMMANDPAYLOAD_MESSAGES \


#endif

#ifdef __cplusplus
} /* extern "C" */
#endif
/* @@protoc_insertion_point(eof) */

#endif
