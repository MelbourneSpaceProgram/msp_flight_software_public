#ifndef SRC_UTIL_SYSTEM_WATCHDOG_H_
#define SRC_UTIL_SYSTEM_WATCHDOG_H_


#include <ti/drivers/Watchdog.h>
#include <ti/drivers/watchdog/WatchdogMSP432.h>

class SystemWatchdog{
  public:
    SystemWatchdog(uint16_t index, uint32_t timeout);
    static void ResetTimer();
  private:
    static uint32_t kTimeoutLength;
    static Watchdog_Handle watchdog_handle;

    static void SetTimeoutLength(uint32_t timeout);
};

#endif //SRC_UTIL_SYSTEM_WATCHDOG_H_
