#include <src/config/satellite.h>
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
        satellite_time.timestamp_ms =
            (uint64_t)epoch_seconds * 1000;
        satellite_time.is_valid = true;
        // TODO(akremor): Pull this from the RTC clock interrupt (once
        // configured)
    } else {
        Log_error0("Unable to convert from RTime -> tm");
        satellite_time.timestamp_ms = NULL;
        satellite_time.is_valid = false;
    }
}

Time SatelliteTimeSource::GetTime() {
    if (kI2cAvailable) {
        if (!satellite_time.is_valid) {
            // Log_error0("Satellite time is not valid");
        }
    } else {
        satellite_time.is_valid = false;
    }

    return satellite_time;
}

uint64_t SatelliteTimeSource::TimeDifferenceMilli(Time start, Time end) {
    return end.timestamp_ms - start.timestamp_ms;
}

void SatelliteTimeSource::RealTimeWait(uint32_t delay_seconds) {
    // The deployment wait acts as a timer counting for `delay_seconds`,
    // starting from the time the `DeploymentWait` call is made.
    // It is a blocking wait.

    if (kInstantDeploymentWaits) {
        Log_info0("Fast-forwarding through deployment wait");
        return;
    }

    uint32_t delay_ms = delay_seconds * kMillisecondsInSecond;

    Time init_time = SatelliteTimeSource::GetTime();
    Time cur_time = init_time;
    bool rtc_time_reliable = false;

    if (init_time.is_valid) {
        rtc_time_reliable = true;
        while ((cur_time.timestamp_ms -
                init_time.timestamp_ms) < delay_ms) {
            cur_time = SatelliteTimeSource::GetTime();

            if (!cur_time.is_valid) {
                rtc_time_reliable = false;
                break;
            }
            TaskUtils::SleepMilli(kDelayCheckInterval);
        }
    }

    if (!rtc_time_reliable) {
        // Fallback to internal clock and reset timer (safest option)
        TaskUtils::SleepMilli(delay_seconds * kMillisecondsInSecond);
    }
}
