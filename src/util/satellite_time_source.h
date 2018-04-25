#ifndef SRC_UTIL_SATELLITE_TIME_SOURCE_H_
#define SRC_UTIL_SATELLITE_TIME_SOURCE_H_

#include <src/messages/Time.pb.h>
#include <src/util/data_types.h>

class SatelliteTimeSource {
   public:
    static void SetTime(RTime time);
    static Time GetTime();
    static uint64_t TimeDifferenceMilli(Time start, Time end);

   private:
    static Time satellite_time;
    static const uint16_t kSatelliteEpochYear = 1970;
};

#endif  // SRC_UTIL_SATELLITE_TIME_SOURCE_H_
