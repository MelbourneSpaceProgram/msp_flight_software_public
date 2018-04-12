#ifndef SRC_UTIL_RUNNABLE_MEMORY_LOGGER_H_
#define SRC_UTIL_RUNNABLE_MEMORY_LOGGER_H_

#include <ti/sysbios/heaps/HeapMem.h>
#include <xdc/runtime/Memory.h>

class RunnableMemoryLogger : public Runnable {
   public:
    fnptr GetRunnablePointer();
    RunnableMemoryLogger();

   private:
    static void LogMemoryStats();
};

#endif  // SRC_UTIL_RUNNABLE_MEMORY_LOGGER_H_
