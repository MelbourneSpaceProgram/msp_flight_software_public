/* Automatically generated nanopb header */
/* Generated by nanopb-0.3.9.1 at Sat Sep 01 13:20:01 2018. */

#ifndef PB_FORMATSDCOMMANDPAYLOAD_PB_H_INCLUDED
#define PB_FORMATSDCOMMANDPAYLOAD_PB_H_INCLUDED
#include <pb.h>

/* @@protoc_insertion_point(includes) */
#if PB_PROTO_HEADER_VERSION != 30
#error Regenerate this file with the current version of nanopb generator.
#endif

#ifdef __cplusplus
extern "C" {
#endif

/* Struct definitions */
typedef struct _FormatSdCommandPayload {
    uint32_t sd_index;
/* @@protoc_insertion_point(struct:FormatSdCommandPayload) */
} FormatSdCommandPayload;

/* Default values for struct fields */

/* Initializer values for message structs */
#define FormatSdCommandPayload_init_default      {0}
#define FormatSdCommandPayload_init_zero         {0}

/* Field tags (for use in manual encoding/decoding) */
#define FormatSdCommandPayload_sd_index_tag      1

/* Struct field encoding specification for nanopb */
extern const pb_field_t FormatSdCommandPayload_fields[2];

/* Maximum encoded size of messages (where known) */
#define FormatSdCommandPayload_size              6

/* Message IDs (where set with "msgid" option) */
#ifdef PB_MSGID

#define FORMATSDCOMMANDPAYLOAD_MESSAGES \


#endif

#ifdef __cplusplus
} /* extern "C" */
#endif
/* @@protoc_insertion_point(eof) */

#endif
