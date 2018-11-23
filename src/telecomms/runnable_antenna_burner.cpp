#include <src/config/satellite.h>
#include <src/database/flash_memory/flash_storables/antenna_burner_info.h>
#include <src/messages/Time.pb.h>
#include <src/telecomms/antenna.h>
#include <src/telecomms/runnable_antenna_burner.h>
#include <src/util/satellite_power.h>
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

    IArg power_key;
    while (1) {
        try {
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
                    // Hold transmission and other power tasks until after the
                    // burn.
                    power_key = SatellitePower::Lock();

                    Antenna::GetAntenna()->DeployAntenna();

                    // If the code has made it to this line, assume success even
                    // if IsDoorsOpen() returns false (which could be possible
                    // if the I2c bus is down)
                    antenna_burner_info->burn_interval_ms *=
                        kAntennaBurnIntervalMultiplier;
                    antenna_burner_info->StoreInFlash();
                    SatellitePower::Unlock(power_key);
                    antenna_burner_info->StoreInFlash();
                }
                TaskUtils::SleepMilli(kAntennaBurnCheckIntervalMs);
            } else {
                TaskUtils::SleepMilli(kBackupAntennaBurnIntervalMs);
                Antenna::GetAntenna()->DeployAntenna();
            }
        } catch (MspException& e) {
            MspException::LogTopLevelException(e, kRunnableAntennaBurnerCatch);
        }
    }
}
