#include <src/util/nanopb_utils.h>

TemperatureReading NanopbUtils::NanopbTemperature(double temperature,
                                                  Time time) {
    TemperatureReading reading = {static_cast<float>(temperature),
                                  time.timestamp_ms};
    return reading;
}

VoltageReading NanopbUtils::NanopbVoltage(double voltage, Time time) {
    VoltageReading reading = {static_cast<float>(voltage),
                              time.timestamp_ms};
    return reading;
}

CurrentReading NanopbUtils::NanopbCurrent(double current, Time time) {
    CurrentReading reading = {static_cast<float>(current),
                              time.timestamp_ms};
    return reading;
}

AccelerometerReading NanopbUtils::NanopbAccelerometer(
    AccelerometerReading accel, Time time) {
    accel.timestamp_ms = time.timestamp_ms;
    return accel;
}

GyroscopeReading NanopbUtils::NanopbGyroscope(GyroscopeReading gyro,
                                              Time time) {
    gyro.timestamp_ms = time.timestamp_ms;
    return gyro;
}

MagnetometerReading NanopbUtils::NanopbMagnetometer(MagnetometerReading mag,
                                                    Time time) {
    mag.timestamp_ms = time.timestamp_ms;
    return mag;
}

BmsReadings NanopbUtils::NanopbBms(BmsReadings bms, Time time) {
    // TODO(akremor): Check if we want to add time to the BMS reading
    // bms.timestamp_ms = time.timestamp_ms;
    return bms;
}
