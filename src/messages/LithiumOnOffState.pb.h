/* Automatically generated nanopb header */
/* Generated by nanopb-0.3.9 at Wed Apr 18 18:47:04 2018. */

#ifndef PB_LITHIUMONOFFSTATE_PB_H_INCLUDED
#define PB_LITHIUMONOFFSTATE_PB_H_INCLUDED
#include <pb.h>

/* @@protoc_insertion_point(includes) */
#if PB_PROTO_HEADER_VERSION != 30
#error Regenerate this file with the current version of nanopb generator.
#endif

#ifdef __cplusplus
extern "C" {
#endif

/* Struct definitions */
typedef struct _LithiumOnOffState {
    bool shouldTransmit;
/* @@protoc_insertion_point(struct:LithiumOnOffState) */
} LithiumOnOffState;

/* Default values for struct fields */

/* Initializer values for message structs */
#define LithiumOnOffState_init_default           {0}
#define LithiumOnOffState_init_zero              {0}

/* Field tags (for use in manual encoding/decoding) */
#define LithiumOnOffState_shouldTransmit_tag     1

/* Struct field encoding specification for nanopb */
extern const pb_field_t LithiumOnOffState_fields[2];

/* Maximum encoded size of messages (where known) */
#define LithiumOnOffState_size                   2

/* Message IDs (where set with "msgid" option) */
#ifdef PB_MSGID

#define LITHIUMONOFFSTATE_MESSAGES \


#endif

#ifdef __cplusplus
} /* extern "C" */
#endif
/* @@protoc_insertion_point(eof) */

#endif
