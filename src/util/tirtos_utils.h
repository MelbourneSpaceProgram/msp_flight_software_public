#ifndef SRC_UTIL_TASK_UTILS_H_
#define SRC_UTIL_TASK_UTILS_H_

#include <stdint.h>
#include <ti/sysbios/gates/GateMutexPri.h>

class MutexLocker {
   public:
    explicit MutexLocker(GateMutexPri_Handle& mutex);
    ~MutexLocker();

    MutexLocker(const MutexLocker&) = delete;
    MutexLocker& operator=(const MutexLocker&) = delete;
    MutexLocker(MutexLocker&&) = delete;
    MutexLocker& operator=(MutexLocker&&) = delete;

   private:
    GateMutexPri_Handle& handle;
    IArg key;
};

class TirtosUtils {
   public:
    static void SleepMilli(uint32_t milliseconds);
    static uint32_t MilliToCycles(uint32_t milliseconds);

   private:
    TirtosUtils();
};

#endif  // SRC_UTIL_TASK_UTILS_H_
