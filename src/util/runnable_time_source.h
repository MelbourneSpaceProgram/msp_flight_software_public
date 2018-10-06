#ifndef SRC_UTIL_RUNNABLE_TIME_SOURCE_H_
#define SRC_UTIL_RUNNABLE_TIME_SOURCE_H_

#include <src/tasks/runnable.h>

class RunnableTimeSource : public Runnable {
   public:
    fnptr GetRunnablePointer();

   private:
    static void UpdateSatelliteTime(uintptr_t arg1, uintptr_t arg2);
};

#endif  // SRC_UTIL_RUNNABLE_TIME_SOURCE_H_
