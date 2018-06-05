#include <src/config/unit_tests.h>
#include <src/sensors/i2c_sensors/rtc.h>
#include <src/util/satellite_time_source.h>
#include <src/util/task_utils.h>
#include <time.h>
#include <xdc/runtime/Log.h>

Time SatelliteTimeSource::satellite_time;

void SatelliteTimeSource::SetTime(RTime time) {
    time_t epoch_seconds = Rtc::RTimeToEpoch(time);

    if (epoch_seconds != (time_t)-1) {
        satellite_time.timestamp_millis_unix_epoch =
            (uint64_t)epoch_seconds * 1000;
        satellite_time.is_valid = true;
        // TODO(akremor): Pull this from the RTC clock interrupt (once
        // configured)
    } else {
        Log_error0("Unable to convert from RTime -> tm");
        satellite_time.timestamp_millis_unix_epoch = NULL;
        satellite_time.is_valid = false;
    }
}

Time SatelliteTimeSource::GetTime() {
    if(i2c_enabled) {
        if (!satellite_time.is_valid) {
            Log_error0("Satellite time is not valid");
        }
    }

    return satellite_time;
}

uint64_t SatelliteTimeSource::TimeDifferenceMilli(Time start, Time end) {
    return end.timestamp_millis_unix_epoch - start.timestamp_millis_unix_epoch;
}
