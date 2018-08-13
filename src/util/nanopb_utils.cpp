#include <src/util/nanopb_utils.h>

TemperatureReading NanopbUtils::NanopbTemperature(double temperature,
                                                  Time time) {
    TemperatureReading reading = {static_cast<float>(temperature),
                                  time.timestamp_millis_unix_epoch};
    return reading;
}

VoltageReading NanopbUtils::NanopbVoltage(double voltage, Time time) {
    VoltageReading reading = {static_cast<float>(voltage),
                              time.timestamp_millis_unix_epoch};
    return reading;
}

CurrentReading NanopbUtils::NanopbCurrent(double current, Time time) {
    CurrentReading reading = {static_cast<float>(current),
                              time.timestamp_millis_unix_epoch};
    return reading;
}

AccelerometerReading NanopbUtils::NanopbAccelerometer(
    AccelerometerReading accel, Time time) {
    accel.timestamp_millis_unix_epoch = time.timestamp_millis_unix_epoch;
    return accel;
}

GyroscopeReading NanopbUtils::NanopbGyroscope(GyroscopeReading gyro,
                                              Time time) {
    gyro.timestamp_millis_unix_epoch = time.timestamp_millis_unix_epoch;
    return gyro;
}

MagnetometerReading NanopbUtils::NanopbMagnetometer(MagnetometerReading mag,
                                                    Time time) {
    mag.timestamp_millis_unix_epoch = time.timestamp_millis_unix_epoch;
    return mag;
}
