/* Automatically generated nanopb constant definitions */
/* Generated by nanopb-0.3.9.1 at Tue Jul 31 21:32:03 2018. */

#include "AccelerometerReading.pb.h"

/* @@protoc_insertion_point(includes) */
#if PB_PROTO_HEADER_VERSION != 30
#error Regenerate this file with the current version of nanopb generator.
#endif



const pb_field_t AccelerometerReading_fields[5] = {
    PB_FIELD(  1, FLOAT   , REQUIRED, STATIC  , FIRST, AccelerometerReading, x, x, 0),
    PB_FIELD(  2, FLOAT   , REQUIRED, STATIC  , OTHER, AccelerometerReading, y, x, 0),
    PB_FIELD(  3, FLOAT   , REQUIRED, STATIC  , OTHER, AccelerometerReading, z, y, 0),
    PB_FIELD(  4, FIXED64 , REQUIRED, STATIC  , OTHER, AccelerometerReading, timestamp_millis_unix_epoch, z, 0),
    PB_LAST_FIELD
};


/* @@protoc_insertion_point(eof) */