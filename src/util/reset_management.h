#ifndef SRC_UTIL_RESET_MANAGEMENT_H
#define SRC_UTIL_RESET_MANAGEMENT_H

#include <src/database/flash_memory/flash_storables/reset_info_container.h>
#include <ti/devices/msp432e4/driverlib/sysctl.h>

class ResetManagement {
   public:
    static void ResetSystemWithMessage(ResetMessage reset_message);
};

#endif  // SRC_UTIL_RESET_MANAGEMENT_H
