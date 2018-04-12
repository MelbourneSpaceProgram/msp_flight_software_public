#include <src/tasks/runnable.h>
#include <src/util/runnable_memory_logger.h>
#include <src/util/task_utils.h>
#include <ti/sysbios/BIOS.h>
#include <ti/sysbios/knl/Task.h>
#include <xdc/runtime/log.h>

RunnableMemoryLogger::RunnableMemoryLogger() {}

fnptr RunnableMemoryLogger::GetRunnablePointer() {
    return &RunnableMemoryLogger::LogMemoryStats;
}

void RunnableMemoryLogger::LogMemoryStats() {
    Memory_Stats mem_stats;
    while (1) {
        HeapMem_getStats(HeapMem_Handle_downCast(Memory_defaultHeapInstance),
                         &mem_stats);
        Log_info3("Heap memory: %d free, %d total, %d largest free",
                  mem_stats.totalFreeSize, mem_stats.totalSize,
                  mem_stats.largestFreeSize);
        Task_sleep(TaskUtils::MilliToCycles(10000));
    }
}
