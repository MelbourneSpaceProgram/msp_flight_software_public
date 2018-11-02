#include <src/board/board.h>
#include <src/config/satellite.h>
#include <src/config/unit_tests.h>
#include <src/init/init.h>
#include <src/init/post_bios_initialiser.h>
#include <src/payload_processor/runnable_payload_processor.h>
#include <src/tasks/task_holder.h>
#include <src/telecomms/runnable_beacon.h>
#include <src/telecomms/runnable_lithium_listener.h>
#include <src/util/satellite_time_source.h>
#include <ti/drivers/GPIO.h>
#include <ti/drivers/I2C.h>
#include <ti/drivers/PWM.h>
#include <ti/drivers/SDFatFS.h>
#include <ti/drivers/SPI.h>
#include <ti/drivers/UART.h>
#include <ti/drivers/Watchdog.h>
#include <xdc/runtime/Log.h>

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

    GPIO_write(SYS_nCS1, 1);
    GPIO_write(SYS_nCS2, 1);
    GPIO_write(SYS_nCS3, 1);

    if (kSdCardAvailable) {
        SDFatFS_init();
    }

    InitSdCs();
    InitRtcInterrupt();

    TaskHolder* post_bios_initialiser_task =
        new TaskHolder(3000, "Initialiser", 10, new PostBiosInitialiser());
    post_bios_initialiser_task->Start();
}

/* Setup rtc pin pulsing interrupt and then enable */
void InitRtcInterrupt() {
    GPIO_setCallback(RTC_INT, SatelliteTimeSource::RtcInterrupt);
    GPIO_enableInt(RTC_INT);
}

void InitSdCs() {
    // Disable second and third SD cards until RAID
    // is written
    GPIO_write(SYS_nCS1, 0);
    GPIO_write(SYS_nCS2, 1);
    GPIO_write(SYS_nCS3, 1);
}

void EnterLimpMode() {
    Log_info0("Entered limp mode");
    initGeneral();
    GPIO_init();
    UART_init();

    Log_info0("Started hardware peripherals");

    // TODO(akremor): Use the GPIO to force burn the antenna (not over I2C)

    // We create task holders here so we have greater control over stacks and
    // priority as we may want to use different parameters for limp mode versus
    // normal mode.
    // For example, we give oversized stacks to minimise the probability of
    // stack corruption.
    TaskHolder* radio_listener = new TaskHolder(5000, "RadioListener", 11,
                                                new RunnableLithiumListener());
    TaskHolder* beacon =
        new TaskHolder(5000, "Beacon", 11, new RunnableBeacon());
    TaskHolder* payload_processor = new TaskHolder(
        5000, "PayloadProcessor", 11, new RunnablePayloadProcessor());

    radio_listener->Start();
    Log_info0("Started radio listener");
    payload_processor->Start();
    Log_info0("Started payload processor");
    beacon->Start();
    Log_info0("Started beacon");

    Log_info0("Limp mode start up complete");

    // TODO(akremor): Possible limp mode features include:
    // - access to sd card to get error logs?
    // - state manager to manage battery health?
    // - what about the 30 minute start up timer?
    // - should the watchdog run?
    // - lithium over-transmit shut off feature?
}
