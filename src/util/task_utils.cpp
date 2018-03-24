#include <src/util/task_utils.h>
#include <ti/sysbios/knl/Clock.h>
#include <ti/sysbios/knl/Task.h>

TaskUtils::TaskUtils() {}

uint32_t TaskUtils::MilliToCycles(uint32_t milliseconds) {
    return milliseconds * 1000 / Clock_tickPeriod;
}

void TaskUtils::SleepMilli(uint32_t milliseconds) {
    Task_sleep(MilliToCycles(milliseconds));
}
