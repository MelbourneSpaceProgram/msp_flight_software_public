#include <src/config/satellite.h>
#include <src/config/unit_tests.h>
#include <src/database/flash_memory/flash_memory_management.h>
#include <src/sensors/i2c_sensors/rtc.h>
#include <src/util/satellite_time_source.h>
#include <src/util/task_utils.h>
#include <time.h>
#include <xdc/runtime/Log.h>

Time SatelliteTimeSource::satellite_time = {0, false};
Time SatelliteTimeSource::initial_time = {0, false};

// TODO(dsutherland): This was the code in HEAD when doing the rebase. Do we
// want to keep this variable volatile?
/* volatile uint64_t SatelliteTimeSource::delta_time = 0; */

uint64_t SatelliteTimeSource::delta_time = 0;
uint64_t SatelliteTimeSource::offset_time = 0;

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
            VerifyTimeOnBoot();
        }

    } else {
        Log_error0("Unable to convert from RTime -> tm");
        satellite_time.timestamp_ms = NULL;
        satellite_time.is_valid = false;
    }
}

/**
 * Verifies that time has not reversed upon being booted up, and if so, revert
 * to last stored time. Note: An offset needs to be stored, which is applied
 * whenever GetTime is called.
 **/
// TODO(dsutherland): Check this. Hugo has added in some logic to deal with
// first wakeup
void SatelliteTimeSource::VerifyTimeOnBoot() {
    // Get the (previously) stored time and offset from flash
    RtcTimeFlash* time_flash = RtcTimeFlash::GetInstance();
    time_flash->UpdateFromFlash();

    if (time_flash->rtc_time ==
        FlashMemoryManagement::kDefaultFlashMemoryDoubleWord) {
        // No time information is stored in the flash (e.g. on first wakeup)
        Time current_time = GetTime();
        time_flash->rtc_time =
            current_time.is_valid
                ? current_time.timestamp_ms
                : FlashMemoryManagement::kDefaultFlashMemoryDoubleWord;
        time_flash->rtc_time_offset = offset_time;
    } else {
        uint64_t flash_time = time_flash->rtc_time;
        offset_time = time_flash->rtc_time_offset;

        // If time has gone backwards
        if (satellite_time.timestamp_ms < flash_time) {
            // Recalculating offset (to be applied whenever GetTime is called
            // since the Rtc is now permanently behind
            offset_time = flash_time - satellite_time.timestamp_ms;
            // Setting time to the last stored value
            satellite_time.timestamp_ms = flash_time;

            // Update offset_time stored on flash
            time_flash->rtc_time_offset = offset_time;
            time_flash->rtc_time = satellite_time.timestamp_ms;
        }
    }
    // Update the timing information stored in the flash
    time_flash->StoreInFlash();
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
    return {satellite_time.timestamp_ms + delta_time + offset_time,
            satellite_time.is_valid};
}

Time SatelliteTimeSource::GetInitialTime() { return initial_time; }

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
        while ((cur_time.timestamp_ms - init_time.timestamp_ms) < delay_ms) {
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
