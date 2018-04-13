#ifndef SRC_UTIL_SATELLITE_TIME_SOURCE_H_
#define SRC_UTIL_SATELLITE_TIME_SOURCE_H_

#include <src/util/data_types.h>

typedef struct SatelliteTime_t {
    bool valid;
    uint32_t epoch_seconds;
    uint16_t milliseconds;
} SatelliteTime;

class SatelliteTimeSource {
   public:
    static void SetTime(RTime time);
    static SatelliteTime GetTime();

   private:
    static SatelliteTime satellite_time;
    static const uint16_t kSatelliteEpochYear = 1970;
};

#endif  // SRC_UTIL_SATELLITE_TIME_SOURCE_H_
