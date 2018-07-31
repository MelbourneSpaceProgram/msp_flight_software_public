/* Automatically generated nanopb header */
/* Generated by nanopb-0.3.9.1 at Tue Jul 31 18:56:14 2018. */

#ifndef PB_VOLTAGEREADING_PB_H_INCLUDED
#define PB_VOLTAGEREADING_PB_H_INCLUDED
#include <pb.h>

/* @@protoc_insertion_point(includes) */
#if PB_PROTO_HEADER_VERSION != 30
#error Regenerate this file with the current version of nanopb generator.
#endif

#ifdef __cplusplus
extern "C" {
#endif

/* Struct definitions */
typedef struct _VoltageReading {
    float voltage;
    uint64_t timestamp_millis_unix_epoch;
/* @@protoc_insertion_point(struct:VoltageReading) */
} VoltageReading;

/* Default values for struct fields */

/* Initializer values for message structs */
#define VoltageReading_init_default              {0, 0}
#define VoltageReading_init_zero                 {0, 0}

/* Field tags (for use in manual encoding/decoding) */
#define VoltageReading_voltage_tag               1
#define VoltageReading_timestamp_millis_unix_epoch_tag 2

/* Struct field encoding specification for nanopb */
extern const pb_field_t VoltageReading_fields[3];

/* Maximum encoded size of messages (where known) */
#define VoltageReading_size                      14

/* Message IDs (where set with "msgid" option) */
#ifdef PB_MSGID

#define VOLTAGEREADING_MESSAGES \


#endif

#ifdef __cplusplus
} /* extern "C" */
#endif
/* @@protoc_insertion_point(eof) */

#endif