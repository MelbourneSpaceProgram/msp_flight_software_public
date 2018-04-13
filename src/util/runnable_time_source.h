#ifndef SRC_UTIL_RUNNABLE_TIME_SOURCE_H_
#define SRC_UTIL_RUNNABLE_TIME_SOURCE_H_

#include <ti/sysbios/heaps/HeapMem.h>
#include <ti/sysbios/knl/Task.h>
#include <xdc/runtime/Memory.h>
#include <xdc/runtime/System.h>
#include <xdc/std.h>

class RunnableTimeSource : public Runnable {
   public:
    fnptr GetRunnablePointer();

   private:
    static void UpdateSatelliteTime();
};

#endif  // SRC_UTIL_RUNNABLE_TIME_SOURCE_H_
