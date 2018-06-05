#ifndef SRC_UTIL_SATELLITE_TIME_SOURCE_H_
#define SRC_UTIL_SATELLITE_TIME_SOURCE_H_

#include <src/messages/Time.pb.h>
#include <src/util/data_types.h>

class SatelliteTimeSource {
   public:
    static void SetTime(RTime time);
    static Time GetTime();
    static uint64_t TimeDifferenceMilli(Time start, Time end);
    static void DeploymentWait(uint32_t delay);

   private:
    static Time satellite_time;
    static const uint16_t kDelayCheckInterval = 1000;
    static const uint16_t kMillisecondsInSecond = 1000;
};

#endif  // SRC_UTIL_SATELLITE_TIME_SOURCE_H_
