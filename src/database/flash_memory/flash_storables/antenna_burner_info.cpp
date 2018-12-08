#include <src/config/satellite.h>
#include <src/database/flash_memory/flash_storables/antenna_burner_info.h>

AntennaBurnerInfo::AntennaBurnerInfo()
    : FlashStorable<AntennaBurnerInfoStruct>(
          kAntennaBurnerInfoFlashStorageAddress),
      last_burn_attempt_timestamp_ms(0),
      burn_interval_ms(SystemConfiguration::GetInstance()->GetInitialAntennaBurnIntervalMs()),
      attempt_antenna_burn(true) {
    UpdateFromFlash();
}

uint64_t AntennaBurnerInfo::GetBurnInterval() { return burn_interval_ms; }

uint64_t AntennaBurnerInfo::GetLastBurnAttempt() {
    return last_burn_attempt_timestamp_ms;
}

bool AntennaBurnerInfo::GetAttemptBurnSetting() { return attempt_antenna_burn; }

void AntennaBurnerInfo::SetBurnInterval(uint64_t new_burn_interval_ms) {
    burn_interval_ms = new_burn_interval_ms;
}

void AntennaBurnerInfo::SetLastBurnAttempt(
    uint64_t new_burn_attempt_timestamp_ms) {
    last_burn_attempt_timestamp_ms = new_burn_attempt_timestamp_ms;
}

void AntennaBurnerInfo::SetAntennaBurnSetting(bool attempt_burn) {
    attempt_antenna_burn = attempt_burn;
}

void AntennaBurnerInfo::UpdateFromFlashStorableStruct(
    AntennaBurnerInfoStruct *antenna_burner_info_struct) {
    last_burn_attempt_timestamp_ms =
        antenna_burner_info_struct->last_burn_attempt_timestamp_ms;
    burn_interval_ms = antenna_burner_info_struct->burn_interval_ms;
    attempt_antenna_burn = antenna_burner_info_struct->attempt_antenna_burn;
}

void AntennaBurnerInfo::ConvertToFlashStorableStruct(
    AntennaBurnerInfoStruct *antenna_burner_info_struct) {
    antenna_burner_info_struct->last_burn_attempt_timestamp_ms =
        last_burn_attempt_timestamp_ms;
    antenna_burner_info_struct->burn_interval_ms = burn_interval_ms;
    antenna_burner_info_struct->attempt_antenna_burn = attempt_antenna_burn;
}
