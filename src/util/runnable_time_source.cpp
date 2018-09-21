#include <src/board/board.h>
#include <src/board/i2c/i2c.h>
#include <src/board/i2c/multiplexers/i2c_multiplexer.h>
#include <src/sensors/i2c_sensors/rtc.h>
#include <src/util/runnable_time_source.h>
#include <src/util/satellite_time_source.h>
#include <src/util/task_utils.h>
#include <xdc/runtime/Log.h>

fnptr RunnableTimeSource::GetRunnablePointer() {
    return &RunnableTimeSource::UpdateSatelliteTime;
}

void RunnableTimeSource::UpdateSatelliteTime() {
    I2c bus(I2C_BUS_A);
    // TODO(akremor): Move this address centrally
    I2cMultiplexer multiplexer(&bus, 0x76);
    Rtc rtc(&bus, 0x69);
    while (1) {
        multiplexer.OpenChannel(I2cMultiplexer::kMuxChannel0);

        RTime time;
        try {
            time = rtc.GetTime();
        } catch (etl::exception& e) {
            Log_error0("Unable to retrieve time from RTC");
            TaskUtils::SleepMilli(5000);
            continue;
        }

        // TODO(akremor): Thread safety issue here
        multiplexer.CloseAllChannels();
        if (rtc.ValidTime(time)) {
            SatelliteTimeSource::SetTime(time);
        }

        TaskUtils::SleepMilli(5000);
    }
}
