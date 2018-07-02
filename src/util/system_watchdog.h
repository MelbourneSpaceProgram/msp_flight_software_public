#ifndef SRC_UTIL_SYSTEM_WATCHDOG_H_
#define SRC_UTIL_SYSTEM_WATCHDOG_H_

#include <ti/drivers/Watchdog.h>
#include <src/board/board.h>

class SystemWatchdog {
   public:
    explicit SystemWatchdog(uint32_t index);
    static void ResetTimer();

   private:
    static Watchdog_Handle watchdog_handle;
};

#endif  // SRC_UTIL_SYSTEM_WATCHDOG_H_
