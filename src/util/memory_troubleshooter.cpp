#include <src/util/memory_troubleshooter.h>

MemoryTroubleshooter::MemoryTroubleshooter() {
    init_mem_stats = GetCurrMemStats();
}

bool MemoryTroubleshooter::MemoryLeakTest() {
    Memory_Stats *end_mem_stats = GetCurrMemStats();

    bool memory_leak = false;

    if (init_mem_stats->totalFreeSize >
        end_mem_stats->totalFreeSize +
            kMemBuff) {
        memory_leak = true;
    }
    delete end_mem_stats;

    return memory_leak;
}

Memory_Stats *MemoryTroubleshooter::GetCurrMemStats() {
    Memory_Stats *mem_stats = new Memory_Stats();

    HeapMem_getStats(HeapMem_Handle_downCast(Memory_defaultHeapInstance),
                     mem_stats);

    return mem_stats;
}

MemoryTroubleshooter::~MemoryTroubleshooter() { delete init_mem_stats; }
