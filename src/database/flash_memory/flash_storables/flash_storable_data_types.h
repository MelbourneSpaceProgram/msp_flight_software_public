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
    kFirstWakeupCause = 0x0000AAAA, // arbitrary value
    kNoResetCause = 0x0000BBBB, // arbitrary value
    kUnknownResetCause = 0x0000CCCC // arbitrary value
};
static constexpr uint32_t kNumResetCauses = 11;
// Must update ResetInfoContainerReading.options if changing the above parameter

// reset messages defined by us to control satellite mode after reset
enum ResetMessage {
    kFirstWakeupMessage = 0,
    kNoResetMessage,
    kForceResetCommandExecuted,
    kResetUnitTestMessage1,
    kResetUnitTestMessage2,
    kUnexpectedReset,
    kNumResetMessages,
    // Must update ResetInfoContainerReading.options if changing the above
    // parameter
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

struct AntennaBurnerInfoStruct {
    uint64_t last_burn_attempt_timestamp_ms;
    uint64_t burn_interval_ms;
    bool attempt_antenna_burn;
};

#endif  // SRC_DATABASE_FLASH_MEMORY_FLASH_STORABLES_FLASH_STORABLE_DATA_TYPES_H_
