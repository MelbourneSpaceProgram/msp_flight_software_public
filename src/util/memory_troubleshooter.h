#ifndef SRC_UTIL_MEMORY_TROUBLESHOOTER_H_
#define SRC_UTIL_MEMORY_TROUBLESHOOTER_H_

#include <ti/sysbios/heaps/HeapMem.h>
#include <xdc/runtime/Memory.h>

class MemoryTroubleshooter {
   public:
    MemoryTroubleshooter();
    bool MemoryLeakTest();
    ~MemoryTroubleshooter();
    static const int kMemBuff = 24;

   private:
    Memory_Stats *GetCurrMemStats();
    Memory_Stats *init_mem_stats;
};

#endif  // SRC_UTIL_MEMORY_TROUBLESHOOTER_H_
