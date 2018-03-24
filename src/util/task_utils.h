#ifndef SRC_UTILS_TASK_UTILS_H_
#define SRC_UTILS_TASK_UTILS_H_

#include <stdint.h>

class TaskUtils {
   public:
    static void SleepMilli(uint32_t milliseconds);
    static uint32_t MilliToCycles(uint32_t milliseconds);

   private:
    TaskUtils();
};

#endif  // SRC_UTILS_TASK_UTILS_H_
