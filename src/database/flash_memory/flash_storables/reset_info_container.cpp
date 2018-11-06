#include <external/etl/exception.h>
#include <src/database/flash_memory/flash_storables/reset_info_container.h>

ResetInfoContainer* ResetInfoContainer::instance = NULL;

ResetInfoContainer::ResetInfoContainer()
    : FlashStorable<ResetInfoContainerStruct>(
          kResetInfoContainerFlashStorageAddress) {
    reset_causes_map[kExternalReset] = 0;
    reset_causes_map[kPowerOnReset] = 1;
    reset_causes_map[kBrownOutReset] = 2;
    reset_causes_map[kWatchdog0Reset] = 3;
    reset_causes_map[kSoftwareReset] = 4;
    reset_causes_map[kWatchdog1Reset] = 5;
    reset_causes_map[kHibernateReset] = 6;
    reset_causes_map[kHardwareSystemServiceRequest] = 7;
    reset_causes_map[kUnknownResetCause] = 8;
    Init();
}

void ResetInfoContainer::Init() {
    UpdateFromFlash();
    StoreInFlash();  // immediately store updated reset data in case the
                     // satellite resets unexpectedly
}

ResetInfoContainer* ResetInfoContainer::GetInstance() {
    if (instance == NULL) {
        instance = new ResetInfoContainer();
    }
    return instance;
}

void ResetInfoContainer::ConvertToFlashStorableStruct(
    ResetInfoContainerStruct* reset_info_container_struct) {
    reset_info_container_struct->num_resets = num_resets;
    reset_info_container_struct->most_recent_reset_cause =
        most_recent_reset_cause;
    for (int16_t i = 0; i < kNumResetCauses; i++) {
        reset_info_container_struct->num_resets_of_cause[i] =
            num_resets_of_cause[i];
    }
    reset_info_container_struct->most_recent_reset_message =
        most_recent_reset_message;
    for (int16_t i = 0; i < kNumResetMessages; i++) {
        reset_info_container_struct->num_resets_with_message[i] =
            num_resets_with_message[i];
    }
    reset_info_container_struct->first_wakeup = first_wakeup;
    reset_info_container_struct->expected_reset = expected_reset;
}

void ResetInfoContainer::UpdateFromFlashStorableStruct(
    ResetInfoContainerStruct* reset_info_container_struct) {
    if (reset_info_container_struct->first_wakeup == kNotFirstWakeup) {
        num_resets = reset_info_container_struct->num_resets;
        num_resets++;
        most_recent_reset_cause = GetMostRecentResetCauseFromFlash();
        for (int16_t i = 0; i < kNumResetCauses; i++) {
            num_resets_of_cause[i] =
                reset_info_container_struct->num_resets_of_cause[i];
        }
        num_resets_of_cause[reset_causes_map[most_recent_reset_cause]]++;

        most_recent_reset_message =
            reset_info_container_struct->expected_reset
                ? reset_info_container_struct->most_recent_reset_message
                : kUnexpectedReset;  // only store the reset message in flash if
                                     // the reset was instigated by the reset
                                     // manager (which asserts
                                     // `expected_reset`); otherwise the message
                                     // could be from a previous reset
        for (int16_t i = 0; i < kNumResetMessages; i++) {
            num_resets_with_message[i] =
                reset_info_container_struct->num_resets_with_message[i];
        }
        num_resets_with_message[static_cast<uint32_t>(
            most_recent_reset_message)]++;

    } else if (reset_info_container_struct->first_wakeup ==
               FlashMemoryManagement::kDefaultFlashMemoryWord) {
        most_recent_reset_cause = GetMostRecentResetCauseFromFlash();
        most_recent_reset_message = kFirstWakeup;
        num_resets = 0;
        for (int16_t i = 0; i < kNumResetCauses; i++) {
            num_resets_of_cause[i] = 0;
        }
        for (int16_t i = 0; i < kNumResetMessages; i++) {
            num_resets_with_message[i] = 0;
        }
    }

    // internal flag variables (which do not track the actual reset state of the
    // satellite)
    first_wakeup = kNotFirstWakeup;
    expected_reset = false;
}

ResetCauses ResetInfoContainer::GetMostRecentResetCause() const {
    return most_recent_reset_cause;
}

uint32_t ResetInfoContainer::GetNumResets() const { return num_resets; }

uint32_t ResetInfoContainer::GetNumResetsOfCause(
    ResetCauses reset_cause) const {
    return num_resets_of_cause[reset_causes_map.at(reset_cause)];
}

uint32_t ResetInfoContainer::GetNumResetsWithMessage(
    ResetMessage reset_message) const {
    return num_resets_with_message[static_cast<uint32_t>(reset_message)];
}

ResetMessage ResetInfoContainer::GetMostRecentResetMessage() const {
    return most_recent_reset_message;
}

ResetCauses ResetInfoContainer::GetMostRecentResetCauseFromFlash() {
    uint32_t most_recent_reset_cause_bit = SysCtlResetCauseGet();
    ResetCauses most_recent_reset_cause =
        IsValidResetCause(most_recent_reset_cause_bit)
            ? static_cast<ResetCauses>(most_recent_reset_cause_bit)
            : kUnknownResetCause;
    return most_recent_reset_cause;
}

bool ResetInfoContainer::IsValidResetCause(uint32_t reset_cause) {
    return reset_cause == kExternalReset || reset_cause == kPowerOnReset ||
           reset_cause == kBrownOutReset || reset_cause == kWatchdog0Reset ||
           reset_cause == kSoftwareReset || reset_cause == kWatchdog1Reset ||
           reset_cause == kHibernateReset ||
           reset_cause == kHardwareSystemServiceRequest ||
           reset_cause == kUnknownResetCause;
    // these values must match the values in the ResetCauses enum
}
