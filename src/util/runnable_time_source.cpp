#include <MSP_EXP432P401R.h>
#include <src/i2c/i2c.h>
#include <src/i2c/multiplexers/i2c_multiplexer.h>
#include <src/sensors/i2c_sensors/rtc.h>
#include <src/tasks/runnable.h>
#include <src/util/runnable_time_source.h>
#include <src/util/satellite_time_source.h>
#include <src/util/task_utils.h>
#include <ti/sysbios/BIOS.h>
#include <ti/sysbios/knl/Task.h>
#include <xdc/runtime/System.h>
#include <xdc/runtime/log.h>
#include <xdc/std.h>

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

        RTime time = rtc.GetTime();

        // TODO(akremor): Thread safety issue here
        multiplexer.CloseAllChannels();
        if (rtc.ValidTime(time)) {
            SatelliteTimeSource::SetTime(time);
        }
        Task_sleep(TaskUtils::MilliToCycles(500));
    }
}
