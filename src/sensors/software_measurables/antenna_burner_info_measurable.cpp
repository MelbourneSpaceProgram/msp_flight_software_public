#include <src/database/flash_memory/flash_storables/antenna_burner_info.h>
#include <src/sensors/software_measurables/antenna_burner_info_measurable.h>
#include <src/telecomms/runnable_antenna_burner.h>

AntennaBurnerInfoMeasurable::AntennaBurnerInfoMeasurable()
    : NanopbMeasurable<AntennaBurnerInfoReading>(
          AntennaBurnerInfoReading_init_default),
      info_string("Antenna burner info"){};

std::string AntennaBurnerInfoMeasurable::GetInfoString() { return info_string; }

AntennaBurnerInfoReading
AntennaBurnerInfoMeasurable::TakeDirectNanopbReading() {
    AntennaBurnerInfo* antenna_burner_info =
        RunnableAntennaBurner::GetAntennaBurnerInfo();

    AntennaBurnerInfoReading reading;
    reading.last_burn_attempt_timestamp_ms =
        antenna_burner_info->GetLastBurnAttempt();
    reading.burn_interval_ms = antenna_burner_info->GetBurnInterval();
    reading.attempt_antenna_burn = antenna_burner_info->GetAttemptBurnSetting();
    return reading;
}
