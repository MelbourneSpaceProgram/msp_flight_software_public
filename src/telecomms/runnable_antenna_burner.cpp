#include <src/config/satellite.h>
#include <src/database/flash_memory/flash_storables/antenna_burner_info.h>
#include <src/messages/Time.pb.h>
#include <src/telecomms/antenna.h>
#include <src/telecomms/runnable_antenna_burner.h>
#include <src/util/satellite_time_source.h>
#include <src/util/task_utils.h>

AntennaBurnerInfo* RunnableAntennaBurner::antenna_burner_info =
    new AntennaBurnerInfo();

fnptr RunnableAntennaBurner::GetRunnablePointer() {
    return &RunnableAntennaBurner::PeriodicAntennaBurner;
};

void RunnableAntennaBurner::PeriodicAntennaBurner() {
    antenna_burner_info->UpdateFromFlash();
    if (antenna_burner_info->last_burn_attempt_timestamp_ms ==
        0xffffffffffffffff) {
        antenna_burner_info->last_burn_attempt_timestamp_ms = 0;
        antenna_burner_info->burn_interval_ms = kInitialAntennaBurnIntervalMs;
    }

    while (1) {
        Time current_time = SatelliteTimeSource::GetTime();
        if (current_time.is_valid) {
            if (current_time.timestamp_ms >
                antenna_burner_info->last_burn_attempt_timestamp_ms +
                    antenna_burner_info->burn_interval_ms) {
                antenna_burner_info->last_burn_attempt_timestamp_ms =
                    current_time.timestamp_ms;
                // Store the record of the attempt before the burn
                // in case the burn kills the power.
                antenna_burner_info->StoreInFlash();
                if (Antenna::GetAntenna()->DeployAntenna()) {
                    antenna_burner_info->burn_interval_ms *=
                        kAntennaBurnIntervalMultiplier;
                    antenna_burner_info->StoreInFlash();
                }
                antenna_burner_info->StoreInFlash();
            }
            TaskUtils::SleepMilli(kDelayCheckIntervalMillis);
        } else {
            TaskUtils::SleepMilli(kBackupAntennaBurnIntervalMs);
            Antenna::GetAntenna()->DeployAntenna();
        }
    }
}
