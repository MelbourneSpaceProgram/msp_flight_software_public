/* Automatically generated nanopb header */
/* Generated by nanopb-0.3.9.1 at Tue Oct 30 15:35:51 2018. */

#ifndef PB_DEPLOYANTENNAUPLINKPAYLOAD_PB_H_INCLUDED
#define PB_DEPLOYANTENNAUPLINKPAYLOAD_PB_H_INCLUDED
#include <pb.h>

/* @@protoc_insertion_point(includes) */
#if PB_PROTO_HEADER_VERSION != 30
#error Regenerate this file with the current version of nanopb generator.
#endif

#ifdef __cplusplus
extern "C" {
#endif

/* Struct definitions */
typedef struct _DeployAntennaUplinkPayload {
    uint32_t deploy_antenna;
/* @@protoc_insertion_point(struct:DeployAntennaUplinkPayload) */
} DeployAntennaUplinkPayload;

/* Default values for struct fields */
extern const uint32_t DeployAntennaUplinkPayload_deploy_antenna_default;

/* Initializer values for message structs */
#define DeployAntennaUplinkPayload_init_default  {0u}
#define DeployAntennaUplinkPayload_init_zero     {0}

/* Field tags (for use in manual encoding/decoding) */
#define DeployAntennaUplinkPayload_deploy_antenna_tag 1

/* Struct field encoding specification for nanopb */
extern const pb_field_t DeployAntennaUplinkPayload_fields[2];

/* Maximum encoded size of messages (where known) */
#define DeployAntennaUplinkPayload_size          5

/* Message IDs (where set with "msgid" option) */
#ifdef PB_MSGID

#define DEPLOYANTENNAUPLINKPAYLOAD_MESSAGES \


#endif

#ifdef __cplusplus
} /* extern "C" */
#endif
/* @@protoc_insertion_point(eof) */

#endif
