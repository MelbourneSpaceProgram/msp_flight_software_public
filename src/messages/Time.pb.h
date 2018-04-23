/* Automatically generated nanopb header */
/* Generated by nanopb-0.3.9 at Mon Apr 23 12:03:19 2018. */

#ifndef PB_TIME_PB_H_INCLUDED
#define PB_TIME_PB_H_INCLUDED
#include <pb.h>

/* @@protoc_insertion_point(includes) */
#if PB_PROTO_HEADER_VERSION != 30
#error Regenerate this file with the current version of nanopb generator.
#endif

#ifdef __cplusplus
extern "C" {
#endif

/* Struct definitions */
typedef struct _Time {
    uint64_t timestamp_millis_unix_epoch;
    bool is_valid;
/* @@protoc_insertion_point(struct:Time) */
} Time;

/* Default values for struct fields */

/* Initializer values for message structs */
#define Time_init_default                        {0, 0}
#define Time_init_zero                           {0, 0}

/* Field tags (for use in manual encoding/decoding) */
#define Time_timestamp_millis_unix_epoch_tag     1
#define Time_is_valid_tag                        2

/* Struct field encoding specification for nanopb */
extern const pb_field_t Time_fields[3];

/* Maximum encoded size of messages (where known) */
#define Time_size                                11

/* Message IDs (where set with "msgid" option) */
#ifdef PB_MSGID

#define TIME_MESSAGES \


#endif

#ifdef __cplusplus
} /* extern "C" */
#endif
/* @@protoc_insertion_point(eof) */

#endif
