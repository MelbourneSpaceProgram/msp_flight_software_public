/* Automatically generated nanopb header */
/* Generated by nanopb-0.3.9.1 at Wed Sep 05 19:32:47 2018. */

#ifndef PB_ENABLEDATALOGGERCOMMANDPAYLOAD_PB_H_INCLUDED
#define PB_ENABLEDATALOGGERCOMMANDPAYLOAD_PB_H_INCLUDED
#include <pb.h>

/* @@protoc_insertion_point(includes) */
#if PB_PROTO_HEADER_VERSION != 30
#error Regenerate this file with the current version of nanopb generator.
#endif

#ifdef __cplusplus
extern "C" {
#endif

/* Struct definitions */
typedef struct _EnableDataloggerCommandPayload {
    bool datalogger_enabled;
/* @@protoc_insertion_point(struct:EnableDataloggerCommandPayload) */
} EnableDataloggerCommandPayload;

/* Default values for struct fields */

/* Initializer values for message structs */
#define EnableDataloggerCommandPayload_init_default {0}
#define EnableDataloggerCommandPayload_init_zero {0}

/* Field tags (for use in manual encoding/decoding) */
#define EnableDataloggerCommandPayload_datalogger_enabled_tag 1

/* Struct field encoding specification for nanopb */
extern const pb_field_t EnableDataloggerCommandPayload_fields[2];

/* Maximum encoded size of messages (where known) */
#define EnableDataloggerCommandPayload_size      2

/* Message IDs (where set with "msgid" option) */
#ifdef PB_MSGID

#define ENABLEDATALOGGERCOMMANDPAYLOAD_MESSAGES \


#endif

#ifdef __cplusplus
} /* extern "C" */
#endif
/* @@protoc_insertion_point(eof) */

#endif