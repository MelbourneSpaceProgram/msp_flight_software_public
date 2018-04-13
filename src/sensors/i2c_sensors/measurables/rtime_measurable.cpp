#include <src/sensors/i2c_sensors/measurables/rtime_measurable.h>

RTimeMeasurable::RTimeMeasurable(Rtc* rtc) : I2cMeasurable<RTime>(rtc) {}

RTime RTimeMeasurable::TakeI2cReading() {
    Rtc* rtc = static_cast<Rtc*>(I2cMeasurable::sensor);
    return rtc->GetTime();
}
