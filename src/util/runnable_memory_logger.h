#ifndef SRC_UTIL_RUNNABLE_MEMORY_LOGGER_H_
#define SRC_UTIL_RUNNABLE_MEMORY_LOGGER_H_

#include <ti/sysbios/heaps/HeapMem.h>
#include <ti/sysbios/knl/Task.h>
#include <xdc/runtime/Memory.h>
#include <xdc/runtime/System.h>
#include <xdc/std.h>

class RunnableMemoryLogger : public Runnable {
   public:
    fnptr GetRunnablePointer();
    RunnableMemoryLogger();

   private:
    static void LogMemoryStats();
    static void PrintTaskInfo(Task_Handle task);
    static constexpr uint32_t kMemoryLoggerRunPeriod = 60000;
};

#endif  // SRC_UTIL_RUNNABLE_MEMORY_LOGGER_H_
