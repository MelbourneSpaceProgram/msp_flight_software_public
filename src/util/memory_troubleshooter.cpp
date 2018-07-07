#include <src/util/memory_troubleshooter.h>

MemoryTroubleshooter::MemoryTroubleshooter() {
    initial_heap_usage = GetCurrentHeapUsage();
}

bool MemoryTroubleshooter::MemoryLeakDetected() {
    xdc_runtime_Memory_Size end_heap_usage = GetCurrentHeapUsage();

    return initial_heap_usage !=
            end_heap_usage;
}

xdc_runtime_Memory_Size MemoryTroubleshooter::GetCurrentHeapUsage() {
    Memory_Stats mem_stats;
    HeapMem_getStats(HeapMem_Handle_downCast(Memory_defaultHeapInstance),
                         &mem_stats);

    return mem_stats.totalSize - mem_stats.totalFreeSize;
}

