#include <src/util/tirtos_utils.h>
#include <ti/sysbios/knl/Clock.h>
#include <ti/sysbios/knl/Task.h>

MutexLocker::MutexLocker(GateMutexPri_Handle& mutex)
    : handle(mutex), key(GateMutexPri_enter(handle)) {}

MutexLocker::~MutexLocker() { GateMutexPri_leave(handle, key); }

TirtosUtils::TirtosUtils() {}

uint32_t TirtosUtils::MilliToCycles(uint32_t milliseconds) {
    return milliseconds * 1000 / Clock_tickPeriod;
}

void TirtosUtils::SleepMilli(uint32_t milliseconds) {
    Task_sleep(MilliToCycles(milliseconds));
}
