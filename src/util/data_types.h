#ifndef SRC_UTIL_DATA_TYPES_H_
#define SRC_UTIL_DATA_TYPES_H_

#include <stddef.h>
#include <stdint.h>

typedef uint8_t byte;

// TODO(dingbenjamin): Create Time class with difference function
typedef struct time {
    byte sec;
    byte min;
    byte hour;
    byte date;
    byte month;
    uint16_t year;
} RTime;

static const double kInvalidDouble = -9999.0;
static const uint16_t kInvalidPositiveInteger = 9999;
static const int16_t kInvalidNegativeInteger = -9999;

#endif  // SRC_UTIL_DATA_TYPES_H_
