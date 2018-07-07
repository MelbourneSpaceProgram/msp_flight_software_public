#ifndef SRC_UTIL_MEMORY_TROUBLESHOOTER_H_
#define SRC_UTIL_MEMORY_TROUBLESHOOTER_H_

#include <ti/sysbios/heaps/HeapMem.h>
#include <xdc/runtime/Memory.h>

class MemoryTroubleshooter {
   public:
    MemoryTroubleshooter();
    bool MemoryLeakDetected();

   private:
    xdc_runtime_Memory_Size GetCurrentHeapUsage();
    xdc_runtime_Memory_Size initial_heap_usage;
};

#endif  // SRC_UTIL_MEMORY_TROUBLESHOOTER_H_
