#include <src/config/satellite.h>
#include <src/config/unit_tests.h>
#include <src/sensors/i2c_sensors/rtc.h>
#include <src/util/satellite_time_source.h>
#include <src/util/tirtos_utils.h>
#include <time.h>
#include <xdc/runtime/Log.h>

Time SatelliteTimeSource::satellite_time = {0, false};
Time SatelliteTimeSource::initial_time = {0, false};
volatile uint64_t SatelliteTimeSource::delta_time = 0;

/**
 * Interrupt called with a frequency of 100hz (i.e. every 10ms)
 */
void SatelliteTimeSource::RtcInterrupt(uint_least8_t index) {
    delta_time += 10;
}

void SatelliteTimeSource::SetTime(RTime time) {
    time_t epoch_seconds = Rtc::RTimeToEpoch(time);

    if (epoch_seconds != (time_t)-1) {
        satellite_time.timestamp_ms = (uint64_t)epoch_seconds * 1000;

        // Resetting delta to 0, since time has just been updated
        delta_time = 0;

        satellite_time.is_valid = true;
        if (!initial_time.is_valid) {
            initial_time = {epoch_seconds, true};
        }
    } else {
        Log_error0("Unable to convert from RTime -> tm");
        satellite_time.timestamp_ms = 0;
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

    // Create a new time object, which uses delta_time (more granular)
    return {satellite_time.timestamp_ms + delta_time, satellite_time.is_valid};
}

Time SatelliteTimeSource::GetInitialTime() { return initial_time; }

uint64_t SatelliteTimeSource::TimeDifferenceMilli(Time start, Time end) {
    return end.timestamp_ms - start.timestamp_ms;
}
