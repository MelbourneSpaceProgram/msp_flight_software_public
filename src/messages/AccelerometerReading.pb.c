/* Automatically generated nanopb constant definitions */
/* Generated by nanopb-0.3.9.1 at Sat Sep 22 20:28:34 2018. */

#include "AccelerometerReading.pb.h"

/* @@protoc_insertion_point(includes) */
#if PB_PROTO_HEADER_VERSION != 30
#error Regenerate this file with the current version of nanopb generator.
#endif

const float AccelerometerReading_x_default = -9999;
const float AccelerometerReading_y_default = -9999;
const float AccelerometerReading_z_default = -9999;
const uint64_t AccelerometerReading_timestamp_ms_default = 0ull;


const pb_field_t AccelerometerReading_fields[5] = {
    PB_FIELD(  1, FLOAT   , REQUIRED, STATIC  , FIRST, AccelerometerReading, x, x, &AccelerometerReading_x_default),
    PB_FIELD(  2, FLOAT   , REQUIRED, STATIC  , OTHER, AccelerometerReading, y, x, &AccelerometerReading_y_default),
    PB_FIELD(  3, FLOAT   , REQUIRED, STATIC  , OTHER, AccelerometerReading, z, y, &AccelerometerReading_z_default),
    PB_FIELD(  4, FIXED64 , REQUIRED, STATIC  , OTHER, AccelerometerReading, timestamp_ms, z, &AccelerometerReading_timestamp_ms_default),
    PB_LAST_FIELD
};


/* @@protoc_insertion_point(eof) */
