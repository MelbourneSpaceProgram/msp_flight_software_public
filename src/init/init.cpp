#include <external/etl/exception.h>
#include <src/board/board.h>
#include <src/board/uart/uart.h>
#include <src/config/satellite.h>
#include <src/config/stacks.h>
#include <src/config/unit_tests.h>
#include <src/database/flash_memory/flash_storables/reset_info_container.h>
#include <src/init/init.h>
#include <src/init/post_bios_initialiser.h>
#include <src/payload_processor/runnable_payload_processor.h>
#include <src/tasks/task_holder.h>
#include <src/telecomms/runnable_beacon.h>
#include <src/telecomms/runnable_lithium_listener.h>
#include <src/util/runnable_console_listener.h>
#include <src/util/runnable_console_logger.h>
#include <src/util/reset_management.h>
#include <src/util/satellite_time_source.h>
#include <src/util/task_utils.h>
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

    if (kSdCardAvailable) {
        SDFatFS_init();
    }

    InitSdCs();

    try {
        ResetInfoContainer* reset_info_container =
            ResetInfoContainer::GetInstance();
        // TODO(hugorilla): Develop this logic further to handle different reset
        // states
        if (reset_info_container->GetNumResets() == kNumResetsForLimpMode) {
            EnterLimpMode();
        } else {
            switch (reset_info_container->GetMostRecentResetMessage()) {
                case kFirstWakeupMessage:
                case kResetUnitTestMessage1:
                case kResetUnitTestMessage2:
                case kForceResetCommandExecuted:
                case kUnexpectedReset:
                    break;
                default:
                    throw etl::exception("Invalid reset flag", __FILE__,
                                         __LINE__);
            }
        }
    } catch (etl::exception& e) {
        EnterLimpMode();
    }

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
    if (!kLimpModeEnabled) {
        return;
    }
    Uart* debug_uart = new Uart(UMBILICAL_CONSOLE);
    debug_uart->SetBaudRate(Uart::kBaud115200)
        ->SetReadTimeout(TaskUtils::MilliToCycles(1000))
        ->SetWriteTimeout(TaskUtils::MilliToCycles(1000))
        ->Open();

    TaskHolder* console_uart_logger_task =
        new TaskHolder(kConsoleLoggerStackSize, "UartLogger", 7,
                       new RunnableConsoleLogger(debug_uart));
    console_uart_logger_task->Start();
    Log_info0("Umbilical UART logger started");

    TaskHolder* console_uart_listener_task =
        new TaskHolder(kConsoleListenerStackSize, "UartListener", 12,
                       new RunnableConsoleListener(debug_uart));
    console_uart_listener_task->Start();
    Log_info0("Umbilical UART listener started");

    Log_info0("Entered limp mode");
    initGeneral();
    GPIO_init();
    UART_init();

    // Re-initialise the section of memory keeping track of satellite resets
    ResetInfoContainer* reset_info_container =
        ResetInfoContainer::GetInstance();
    reset_info_container->InitialiseValues();
    reset_info_container->StoreInFlash();

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
