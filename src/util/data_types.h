#ifndef SRC_UTIL_DATA_TYPES_H_
#define SRC_UTIL_DATA_TYPES_H_

#include <stddef.h>
#include <stdint.h>

typedef uint8_t byte;

typedef struct time {
    byte sec;
    byte min;
    byte hour;
    byte date;
    byte month;
    uint16_t year;
} RTime;

#endif  // SRC_UTIL_DATA_TYPES_H_
