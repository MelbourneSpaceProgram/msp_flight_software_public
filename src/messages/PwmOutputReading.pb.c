/* Automatically generated nanopb constant definitions */
/* Generated by nanopb-0.3.9.1 at Sat Sep 22 20:28:34 2018. */

#include "PwmOutputReading.pb.h"

/* @@protoc_insertion_point(includes) */
#if PB_PROTO_HEADER_VERSION != 30
#error Regenerate this file with the current version of nanopb generator.
#endif

const double PwmOutputReading_x_default = -9999;
const double PwmOutputReading_y_default = -9999;
const double PwmOutputReading_z_default = -9999;
const uint64_t PwmOutputReading_timestamp_ms_default = 0ull;


const pb_field_t PwmOutputReading_fields[5] = {
    PB_FIELD(  1, DOUBLE  , REQUIRED, STATIC  , FIRST, PwmOutputReading, x, x, &PwmOutputReading_x_default),
    PB_FIELD(  2, DOUBLE  , REQUIRED, STATIC  , OTHER, PwmOutputReading, y, x, &PwmOutputReading_y_default),
    PB_FIELD(  3, DOUBLE  , REQUIRED, STATIC  , OTHER, PwmOutputReading, z, y, &PwmOutputReading_z_default),
    PB_FIELD(  4, FIXED64 , REQUIRED, STATIC  , OTHER, PwmOutputReading, timestamp_ms, z, &PwmOutputReading_timestamp_ms_default),
    PB_LAST_FIELD
};


/* On some platforms (such as AVR), double is really float.
 * These are not directly supported by nanopb, but see example_avr_double.
 * To get rid of this error, remove any double fields from your .proto.
 */
PB_STATIC_ASSERT(sizeof(double) == 8, DOUBLE_MUST_BE_8_BYTES)

/* @@protoc_insertion_point(eof) */
