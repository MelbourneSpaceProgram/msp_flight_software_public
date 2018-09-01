#include <src/board/board.h>
#include <src/config/satellite.h>
#include <src/config/unit_tests.h>
#include <src/init/init.h>
#include <src/init/post_bios_initialiser.h>
#include <src/tasks/task_holder.h>
#include <ti/drivers/GPIO.h>
#include <ti/drivers/I2C.h>
#include <ti/drivers/PWM.h>
#include <ti/drivers/SDFatFS.h>
#include <ti/drivers/SPI.h>
#include <ti/drivers/UART.h>
#include <ti/drivers/Watchdog.h>

// Initialises the core MSP432 drivers provided by TI and loads the post BIOS
// init tasks. Should be called once at system startup, and prior to the BIOS
// starting.
void PreBiosInit() {
    if (kEnterDeepSleepOnStartup) {
        // Spin for a bit in case we need to stop this happening with the
        // debugger etc (conservative approach, this likely isn't needed).
        // Will take around 1-30 seconds to finish this loop; depends on things
        // like system clock frequency, and whether the debugger or breakpoints
        // are loaded. This isn't designed to be timing level accurate.
        volatile uint32_t timeout = 120E6;
        for (; timeout > 0; timeout--)
            ;

        EnterLowPowerMode();
    }

    initGeneral();
    GPIO_init();
    I2C_init();
    SPI_init();
    UART_init();
    PWM_init();

    if (kSdCardAvailable) {
        SDFatFS_init();
    }

    TaskHolder *post_bios_initialiser_task =
        new TaskHolder(3000, "Initialiser", 10, new PostBiosInitialiser());
    post_bios_initialiser_task->Start();
}
