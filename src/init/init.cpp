#include <src/board/board.h>
#include <src/board/uart/uart.h>
#include <src/config/satellite.h>
#include <src/config/stacks.h>
#include <src/database/flash_memory/flash_storables/reset_info_container.h>
#include <src/init/init.h>
#include <src/init/limp_mode_initialiser.h>
#include <src/init/post_bios_initialiser.h>
#include <src/payload_processor/runnable_payload_processor.h>
#include <src/tasks/task_holder.h>
#include <src/telecomms/runnable_beacon.h>
#include <src/telecomms/runnable_lithium_listener.h>
#include <src/util/msp_exception.h>
#include <src/util/reset_management.h>
#include <src/util/runnable_console_listener.h>
#include <src/util/runnable_console_logger.h>
#include <src/util/satellite_time_source.h>
#include <src/util/system_watchdog.h>
#include <src/util/tirtos_utils.h>
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
    if (SystemConfiguration::GetInstance()->IsEnterDeepSleepOnStartup()) {
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

    SysCtlDelay(20E6);  // Externally activated hibernation window

    initGeneral();
    GPIO_init();
    UART_init();

    if (SystemConfiguration::GetInstance()->IsSdCardAvailable()) {
        SDFatFS_init();
    }

    InitSdCs();

    try {
        ResetInfoContainer* reset_info_container =
            ResetInfoContainer::GetInstance();
        // TODO(hugorilla): Develop this logic further to handle different reset
        // states
        if (reset_info_container->GetNumResets() == kNumResetsForLimpMode ||
            SystemConfiguration::GetInstance()->IsEnterLimpModeOnStartup()) {
            EnterLimpMode();
        } else {
            switch (reset_info_container->GetMostRecentResetMessage()) {
                case kFirstWakeupMessage:
                    if (SystemConfiguration::GetInstance()->IsVerboseLogging())
                        Log_info0("Reset Message: First Wakeup");
                    break;
                case kResetUnitTestMessage1:
                    if (SystemConfiguration::GetInstance()->IsVerboseLogging())
                        Log_info0("Reset Message: Unit Test 1");
                    break;
                case kResetUnitTestMessage2:
                    if (SystemConfiguration::GetInstance()->IsVerboseLogging())
                        Log_info0("Reset Message: Unit Test 2");
                    break;
                case kForceResetCommandExecuted:
                    if (SystemConfiguration::GetInstance()->IsVerboseLogging())
                        Log_info0("Reset Message: Force Reset");
                    break;
                case kUnexpectedReset:
                    if (SystemConfiguration::GetInstance()->IsVerboseLogging())
                        Log_info0("Reset Message: Unexpected Reset");
                    break;
                default:
                    throw MspException("Invalid reset flag",
                                       kInvalidResetFlagFail, __FILE__,
                                       __LINE__);
            }
            I2C_init();
            SPI_init();
            PWM_init();

            TaskHolder* post_bios_initialiser_task = new TaskHolder(
                3000, "Initialiser", 10, new PostBiosInitialiser());
            post_bios_initialiser_task->Start();
        }
    } catch (MspException& e) {
        // TODO(dingbenjamin): Save this somehow when MspException is not yet
        // initialised
        MspException::LogException(e, kBootResetInfoCatch);
        EnterLimpMode();
    }

    // TODO(dingbenjamin): Log the reset counts for each type to console
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
    TaskHolder* limp_mode_initialiser_task =
        new TaskHolder(3000, "Initialiser", 10, new LimpModeInitialiser());
    limp_mode_initialiser_task->Start();
}
