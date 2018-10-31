/* Automatically generated nanopb header */
/* Generated by nanopb-0.3.9.1 at Wed Oct 31 16:22:09 2018. */

#ifndef PB_SECTIONUPLINKPAYLOAD_PB_H_INCLUDED
#define PB_SECTIONUPLINKPAYLOAD_PB_H_INCLUDED
#include <pb.h>

/* @@protoc_insertion_point(includes) */
#if PB_PROTO_HEADER_VERSION != 30
#error Regenerate this file with the current version of nanopb generator.
#endif

#ifdef __cplusplus
extern "C" {
#endif

/* Struct definitions */
typedef PB_BYTES_ARRAY_T(128) SectionUplinkPayload_section_t;
typedef struct _SectionUplinkPayload {
    uint32_t command_code;
    uint32_t start_index;
    uint32_t end_index;
    SectionUplinkPayload_section_t section;
/* @@protoc_insertion_point(struct:SectionUplinkPayload) */
} SectionUplinkPayload;

/* Default values for struct fields */

/* Initializer values for message structs */
#define SectionUplinkPayload_init_default        {0, 0, 0, {0, {0}}}
#define SectionUplinkPayload_init_zero           {0, 0, 0, {0, {0}}}

/* Field tags (for use in manual encoding/decoding) */
#define SectionUplinkPayload_command_code_tag    1
#define SectionUplinkPayload_start_index_tag     2
#define SectionUplinkPayload_end_index_tag       3
#define SectionUplinkPayload_section_tag         4

/* Struct field encoding specification for nanopb */
extern const pb_field_t SectionUplinkPayload_fields[5];

/* Maximum encoded size of messages (where known) */
#define SectionUplinkPayload_size                149

/* Message IDs (where set with "msgid" option) */
#ifdef PB_MSGID

#define SECTIONUPLINKPAYLOAD_MESSAGES \


#endif

#ifdef __cplusplus
} /* extern "C" */
#endif
/* @@protoc_insertion_point(eof) */

#endif
