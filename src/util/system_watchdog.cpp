#include <src/util/system_watchdog.h>
#include <xdc/runtime/Log.h>

Watchdog_Handle SystemWatchdog::watchdog_handle = NULL;
uint32_t SystemWatchdog::kTimeoutLength = 0xFFFFFFFF;

SystemWatchdog::SystemWatchdog(uint16_t index, uint32_t timeout){
    Watchdog_init();
    Watchdog_Params params;

    kTimeoutLength = timeout;

    Watchdog_Params_init(&params);
    watchdog_handle = Watchdog_open(index, &params);

    if(!watchdog_handle){
        Log_error0("Watchdog failed to open");
    }
}

void SystemWatchdog::ResetTimer(){
    if (!watchdog_handle){
        Log_error0("Calling reset on uninitialised watchdog");
        return;
    }
    Watchdog_clear(watchdog_handle);
}
