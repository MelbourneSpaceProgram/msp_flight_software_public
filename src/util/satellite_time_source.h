#ifndef SRC_UTIL_SATELLITE_TIME_SOURCE_H_
#define SRC_UTIL_SATELLITE_TIME_SOURCE_H_

#include <src/messages/Time.pb.h>
#include <src/util/data_types.h>

class SatelliteTimeSource {
   public:
    static void SetTime(RTime time);
    static Time GetTime();
    static Time GetInitialTime();
    static uint64_t TimeDifferenceMilli(Time start, Time end);
    static void RealTimeWait(uint32_t delay);
    static void RtcInterrupt(uint_least8_t index);
    static uint64_t delta_time;  // time (in millis) between calls to the rtc

   private:
    static Time satellite_time;
    static Time initial_time;
    static constexpr uint16_t kDelayCheckInterval = 1000;
    static constexpr uint16_t kMillisecondsInSecond = 1000;
};

#endif  // SRC_UTIL_SATELLITE_TIME_SOURCE_H_
