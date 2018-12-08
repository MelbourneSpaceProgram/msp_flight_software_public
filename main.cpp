#include <src/config/satellite.h>
#include <src/init/init.h>
#include <src/util/runnable_console_logger.h>
#include <ti/devices/msp432e4/driverlib/sysctl.h>
#include <ti/sysbios/BIOS.h>
#include <xdc/runtime/System.h>

extern "C" {
void Abort() {
    if (kFlushUartOnHardFault) FullUartFlush();
    SysCtlReset();
}

void Exit(int arg) {
    if (kFlushUartOnHardFault) FullUartFlush();
    SysCtlReset();
}
}

int main(void) {
    PreBiosInit();
    BIOS_start();  // Infinite Loop
    return (0);
}
