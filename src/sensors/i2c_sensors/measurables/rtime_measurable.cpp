#include <src/sensors/i2c_sensors/measurables/rtime_measurable.h>

const RTime RTimeMeasurable::kFailedRTimeReading = {0, 0, 0, 0, 0, 0};

RTimeMeasurable::RTimeMeasurable(Rtc* rtc)
    : I2cMeasurable<RTime>(rtc, kFailedRTimeReading) {}

RTime RTimeMeasurable::TakeDirectI2cReading() {
    Rtc* rtc = static_cast<Rtc*>(I2cMeasurable::sensor);
    return rtc->GetTime();
}
