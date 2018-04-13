#include <src/util/satellite_time_source.h>
#include <time.h>
#include <xdc/runtime/log.h>

SatelliteTime SatelliteTimeSource::satellite_time;

void SatelliteTimeSource::SetTime(RTime time) {
    tm converted_time;
    converted_time.tm_year = time.year - kSatelliteEpochYear;
    converted_time.tm_mon = time.month - 1;  // RTime uses 1 based month indices
    converted_time.tm_mday = time.date;
    converted_time.tm_hour = time.hour;
    converted_time.tm_min = time.min;
    converted_time.tm_sec = time.sec;

    time_t epoch_seconds;
    if ((epoch_seconds = std::mktime(&converted_time)) != (time_t)-1) {
        satellite_time.epoch_seconds = epoch_seconds;
        // TODO(akremor): Pull this from the RTC clock interrupt (once
        // configured)
        satellite_time.milliseconds = 0;
        satellite_time.valid = true;
        Log_info1("Satellite time set to %lld", satellite_time.epoch_seconds);
    } else {
        Log_error0("Unable to convert from RTime -> tm");
        satellite_time.epoch_seconds = 0;
        satellite_time.milliseconds = 0;
        satellite_time.valid = false;
    }
}

SatelliteTime SatelliteTimeSource::GetTime() {
    if (!satellite_time.valid) {
        Log_error0("Satellite time has not been initialised");
    }

    return satellite_time;
}
