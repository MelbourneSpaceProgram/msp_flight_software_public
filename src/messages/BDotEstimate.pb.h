/* Automatically generated nanopb header */
/* Generated by nanopb-0.3.9 at Thu Aug 16 11:46:48 2018. */

#ifndef PB_BDOTESTIMATE_PB_H_INCLUDED
#define PB_BDOTESTIMATE_PB_H_INCLUDED
#include <pb.h>

/* @@protoc_insertion_point(includes) */
#if PB_PROTO_HEADER_VERSION != 30
#error Regenerate this file with the current version of nanopb generator.
#endif

#ifdef __cplusplus
extern "C" {
#endif

/* Struct definitions */
typedef struct _BDotEstimate {
    double x;
    double y;
    double z;
    uint64_t timestamp_millis_unix_epoch;
/* @@protoc_insertion_point(struct:BDotEstimate) */
} BDotEstimate;

/* Default values for struct fields */

/* Initializer values for message structs */
#define BDotEstimate_init_default                {0, 0, 0, 0}
#define BDotEstimate_init_zero                   {0, 0, 0, 0}

/* Field tags (for use in manual encoding/decoding) */
#define BDotEstimate_x_tag                       1
#define BDotEstimate_y_tag                       2
#define BDotEstimate_z_tag                       3
#define BDotEstimate_timestamp_millis_unix_epoch_tag 4

/* Struct field encoding specification for nanopb */
extern const pb_field_t BDotEstimate_fields[5];

/* Maximum encoded size of messages (where known) */
#define BDotEstimate_size                        36

/* Message IDs (where set with "msgid" option) */
#ifdef PB_MSGID

#define BDOTESTIMATE_MESSAGES \


#endif

#ifdef __cplusplus
} /* extern "C" */
#endif
/* @@protoc_insertion_point(eof) */

#endif
