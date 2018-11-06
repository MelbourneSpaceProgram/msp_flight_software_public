#include <src/database/flash_memory/flash_storables/reset_info_container.h>
#include <src/util/reset_management.h>

void ResetManagement::ResetSystemWithMessage(ResetMessage reset_message) {
    ResetInfoContainer *reset_info_container =
        ResetInfoContainer::GetInstance();
    reset_info_container->most_recent_reset_message = reset_message;
    reset_info_container->expected_reset = true;
    reset_info_container->StoreInFlash();

    // clear the reset cause register
    SysCtlResetCauseClear(SysCtlResetCauseGet());

    // reset the satellite
    SysCtlReset();
}
