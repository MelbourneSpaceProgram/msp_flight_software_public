/* Automatically generated nanopb header */
/* Generated by nanopb-0.3.9 at Thu Mar 22 22:03:31 2018. */

#ifndef PB_SENSORREADING_PB_H_INCLUDED
#define PB_SENSORREADING_PB_H_INCLUDED
#include <pb.h>

/* @@protoc_insertion_point(includes) */
#if PB_PROTO_HEADER_VERSION != 30
#error Regenerate this file with the current version of nanopb generator.
#endif

#ifdef __cplusplus
extern "C" {
#endif

/* Struct definitions */
typedef struct _SensorReading {
    double value;
    uint64_t timestamp_millis_unix_epoch;
/* @@protoc_insertion_point(struct:SensorReading) */
} SensorReading;

/* Default values for struct fields */

/* Initializer values for message structs */
#define SensorReading_init_default               {0, 0}
#define SensorReading_init_zero                  {0, 0}

/* Field tags (for use in manual encoding/decoding) */
#define SensorReading_value_tag                  1
#define SensorReading_timestamp_millis_unix_epoch_tag 2

/* Struct field encoding specification for nanopb */
extern const pb_field_t SensorReading_fields[3];

/* Maximum encoded size of messages (where known) */
#define SensorReading_size                       18

/* Message IDs (where set with "msgid" option) */
#ifdef PB_MSGID

#define SENSORREADING_MESSAGES \


#endif

#ifdef __cplusplus
} /* extern "C" */
#endif
/* @@protoc_insertion_point(eof) */

#endif
