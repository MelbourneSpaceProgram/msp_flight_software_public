#include <src/util/tirtos_utils.h>
#include <ti/sysbios/knl/Clock.h>
#include <ti/sysbios/knl/Task.h>

TirtosUtils::TirtosUtils() {}

uint32_t TirtosUtils::MilliToCycles(uint32_t milliseconds) {
    return milliseconds * 1000 / Clock_tickPeriod;
}

void TirtosUtils::SleepMilli(uint32_t milliseconds) {
    Task_sleep(MilliToCycles(milliseconds));
}
