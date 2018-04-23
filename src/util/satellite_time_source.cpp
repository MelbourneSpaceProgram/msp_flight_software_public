#include <src/util/satellite_time_source.h>
#include <time.h>
#include <xdc/runtime/Log.h>

Time SatelliteTimeSource::satellite_time;

void SatelliteTimeSource::SetTime(RTime time) {
    tm converted_time;
    converted_time.tm_year = time.year - kSatelliteEpochYear;
    converted_time.tm_mon = time.month - 1;  // RTime uses 1 based month indices
    converted_time.tm_mday = time.date;
    converted_time.tm_hour = time.hour;
    converted_time.tm_min = time.min;
    converted_time.tm_sec = time.sec;

    time_t epoch_seconds = std::mktime(&converted_time);
    if (epoch_seconds != (time_t)-1) {
        satellite_time.timestamp_millis_unix_epoch =
            (uint64_t)epoch_seconds * 1000;
        // TODO(akremor): Pull this from the RTC clock interrupt (once
        // configured)
    } else {
        Log_error0("Unable to convert from RTime -> tm");
        satellite_time.timestamp_millis_unix_epoch = NULL;
    }
}

Time SatelliteTimeSource::GetTime() {
    if (!satellite_time.timestamp_millis_unix_epoch) {
        Log_error0("Satellite time has not been set");
    }

    return satellite_time;
}
