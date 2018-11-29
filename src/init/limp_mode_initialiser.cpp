#include <src/board/board.h>
#include <src/board/uart/uart.h>
#include <src/config/satellite.h>
#include <src/database/flash_memory/flash_storables/reset_info_container.h>
#include <src/init/limp_mode_initialiser.h>
#include <src/init/post_bios_initialiser.h>
#include <src/payload_processor/runnable_payload_processor.h>
#include <src/sensors/measurable_manager.h>
#include <src/tasks/task_holder.h>
#include <src/telecomms/antenna.h>
#include <src/telecomms/lithium.h>
#include <src/telecomms/runnable_antenna_burner.h>
#include <src/telecomms/runnable_beacon.h>
#include <src/telecomms/runnable_lithium_listener.h>
#include <src/util/system_watchdog.h>
#include <src/util/tirtos_utils.h>
#include <ti/drivers/GPIO.h>
#include <ti/drivers/UART.h>
#include <ti/drivers/Watchdog.h>
#include <xdc/runtime/Log.h>

LimpModeInitialiser::LimpModeInitialiser() {}

fnptr LimpModeInitialiser::GetRunnablePointer() {
    return &LimpModeInitialiser::LimpModeInit;
}

void LimpModeInitialiser::LimpModeInit() {
    // It is desired to have greater control over stacks and
    // priority as we may want to use different parameters for limp mode versus
    // normal mode.
    // For example, we give oversized stacks to minimise the probability of
    // stack corruption.
    // TODO(dingbenjamin): Parameterize task priority
    // TODO(dingbenjamin): Limp mode antenna burn without using I2c
    // TODO(dingbenjamin): Mute log messages on gracefully failed I2c
    // transactions

    GPIO_write(nCOMMS_RST,
               1);  // Turn on telecomms without initialising SatellitePower

    PostBiosInitialiser::InitConsoleUart(5000, 5000);
    Log_info0("Entered limp mode and started hardware peripherals");

    // Re-initialise the section of memory keeping track of satellite resets
    ResetInfoContainer* reset_info_container =
        ResetInfoContainer::GetInstance();
    reset_info_container->InitialiseValues();
    reset_info_container->StoreInFlash();

    PostBiosInitialiser::InitRadioListener(5000);
    PostBiosInitialiser::InitPayloadProcessor(10000);
    SystemWatchdog((uint32_t)SYS_WATCHDOG0);

    PostBiosInitialiser::BeaconWait();
    PostBiosInitialiser::InitBeacon(5000, true);

    Log_info0("Limp mode start up complete");
}
