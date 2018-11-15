#ifndef SRC_UTIL_RUNNABLE_TIME_SOURCE_H_
#define SRC_UTIL_RUNNABLE_TIME_SOURCE_H_

#include <src/tasks/runnable.h>

class RunnableTimeSource : public Runnable {
   public:
    fnptr GetRunnablePointer();

   private:
    static void UpdateSatelliteTime();
    static constexpr uint8_t kRtcAddress = 0x69;
    static constexpr uint8_t kMuxAddress = 0x76;
};

#endif  // SRC_UTIL_RUNNABLE_TIME_SOURCE_H_
