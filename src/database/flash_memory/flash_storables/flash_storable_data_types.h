#ifndef SRC_DATABASE_FLASH_MEMORY_FLASH_STORABLES_FLASH_STORABLE_DATA_TYPES_H_
#define SRC_DATABASE_FLASH_MEMORY_FLASH_STORABLES_FLASH_STORABLE_DATA_TYPES_H_

/* This file is a list of the structs for storing flash storable objects in
 * flash memory. Struct members should correspond to the members of the original
 * flash storable object.
 */

#include <ti/devices/msp432e4/driverlib/sysctl.h>

// reset causes defined by the MSP432E4 board
enum ResetCauses {
    kExternalReset = SYSCTL_CAUSE_EXT,
    kPowerOnReset = SYSCTL_CAUSE_POR,
    kBrownOutReset = SYSCTL_CAUSE_BOR,
    kWatchdog0Reset = SYSCTL_CAUSE_WDOG0,
    kSoftwareReset = SYSCTL_CAUSE_SW,
    kWatchdog1Reset = SYSCTL_CAUSE_WDOG1,
    kHibernateReset = SYSCTL_CAUSE_HIB,
    kHardwareSystemServiceRequest = SYSCTL_CAUSE_HSRVREQ,
    kUnknownResetCause = 0x0000AAAA
};
static constexpr uint32_t kNumResetCauses = 9;

// reset messages defined by us to control satellite mode after reset
enum ResetMessage {
    kFirstWakeup = 0,
    kForceResetCommandExecuted,
    kResetUnitTestMessage1,
    kResetUnitTestMessage2,
    kUnexpectedReset,
    kNumResetMessages,
};

struct ResetInfoContainerStruct {
    ResetCauses most_recent_reset_cause;
    ResetMessage most_recent_reset_message;
    uint32_t num_resets;
    uint32_t num_resets_of_cause[kNumResetCauses];
    uint32_t num_resets_with_message[kNumResetMessages];
    uint32_t first_wakeup;
    bool expected_reset;
};

#endif  // SRC_DATABASE_FLASH_MEMORY_FLASH_STORABLES_FLASH_STORABLE_DATA_TYPES_H_
