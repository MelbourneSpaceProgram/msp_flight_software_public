#ifndef SRC_UTIL_SATELLITE_TIME_SOURCE_H_
#define SRC_UTIL_SATELLITE_TIME_SOURCE_H_

#include <src/messages/Time.pb.h>
#include <src/util/data_types.h>
#include "../database/flash_memory/flash_storables/rtc_time_flash.h"

class SatelliteTimeSource {
   public:
    static void SetTime(RTime time);
    static Time GetTime();
    static Time GetInitialTime();
    static uint64_t TimeDifferenceMilli(Time start, Time end);
    static void RealTimeWait(uint32_t delay);
    static void RtcInterrupt(uint_least8_t index);

    // TODO(dsutherland): This was the code in HEAD when doing the rebase. Do we
    // want to keep this variable volatile?
    static volatile uint64_t
        delta_time;  // time (in millis) between calls to the rtc

    static void VerifyTimeOnBoot();
    // static uint64_t delta_time;  // time (in millis) between calls to the rtc

   private:
    static Time satellite_time;
    static Time initial_time;

    // Time (millis) which may exist because of reboots causing rtc time to go
    // backwards
    static uint64_t offset_time;
    static constexpr uint16_t kDelayCheckInterval = 1000;
    static constexpr uint16_t kMillisecondsInSecond = 1000;
};

#endif  // SRC_UTIL_SATELLITE_TIME_SOURCE_H_
