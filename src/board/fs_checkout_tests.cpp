#include <CppUTest/TestHarness.h>
#include <src/config/unit_tests.h>
#include <src/messages/CurrentReading.pb.h>
#include <src/messages/TemperatureReading.pb.h>
#include <src/messages/VoltageReading.pb.h>
#include <src/sensors/measurable_manager.h>
#include <src/sensors/i2c_sensors/measurables/imu_temperature_measurable.h>

TEST_GROUP(FsCheckout) {
    void setup() {
        if (!kFsBoardAvailable) {
            TEST_EXIT
        }
    };
};

static MeasurableManager* measurable_manager =
    MeasurableManager::GetInstance();

TEST(FsCheckout, CanSenseHBridgeXTemperature) {
    TemperatureReading fs_hb_xt =
        measurable_manager->ReadNanopbMeasurable<TemperatureReading>(kFsHbXT,
                                                                      0);
    DOUBLES_EQUAL(20, fs_hb_xt.temp, 45);
}

TEST(FsCheckout, CanSenseHBridgeYTemperature) {
    TemperatureReading fs_hb_yt =
        measurable_manager->ReadNanopbMeasurable<TemperatureReading>(kFsHbYT,
                                                                      0);
    DOUBLES_EQUAL(20, fs_hb_yt.temp, 45);
}

TEST(FsCheckout, CanSenseHBridgeZTemperature) {
    TemperatureReading fs_hb_zt =
        measurable_manager->ReadNanopbMeasurable<TemperatureReading>(kFsHbZT,
                                                                      0);
    DOUBLES_EQUAL(20, fs_hb_zt.temp, 45);
}

TEST(FsCheckout, CanSenseMagTorqXVoltage) {
    VoltageReading voltage_a =
        measurable_manager->ReadNanopbMeasurable<VoltageReading>(kFsTorquerXAV,
                                                                  0);
    DOUBLES_EQUAL(2.5, voltage_a.voltage, 3);

    VoltageReading voltage_b =
        measurable_manager->ReadNanopbMeasurable<VoltageReading>(kFsTorquerXBV,
                                                                  0);
    DOUBLES_EQUAL(2.5, voltage_b.voltage, 3);
}

TEST(FsCheckout, CanSenseMagTorqYVoltage) {
    VoltageReading voltage_a =
        measurable_manager->ReadNanopbMeasurable<VoltageReading>(kFsTorquerYAV,
                                                                  0);
    DOUBLES_EQUAL(2.5, voltage_a.voltage, 3);

    VoltageReading voltage_b =
        measurable_manager->ReadNanopbMeasurable<VoltageReading>(kFsTorquerYBV,
                                                                  0);
    DOUBLES_EQUAL(2.5, voltage_b.voltage, 3);
}

TEST(FsCheckout, CanSenseMagTorqZVoltage) {
    VoltageReading voltage_a =
        measurable_manager->ReadNanopbMeasurable<VoltageReading>(kFsTorquerZAV,
                                                                  0);
    DOUBLES_EQUAL(2.5, voltage_a.voltage, 3);

    VoltageReading voltage_b =
        measurable_manager->ReadNanopbMeasurable<VoltageReading>(kFsTorquerZBV,
                                                                  0);
    DOUBLES_EQUAL(2.5, voltage_b.voltage, 3);
}

TEST(FsCheckout, Imu1) {
    TemperatureReading temperature =
        measurable_manager->ReadNanopbMeasurable<TemperatureReading>(kFsImuT1,
                                                                      0);

    DOUBLES_EQUAL(25, temperature.temp, 45);
}

TEST(FsCheckout, Imu2) {
    TemperatureReading temperature =
        measurable_manager->ReadNanopbMeasurable<TemperatureReading>(kFsImuT2,
                                                                      0);

    DOUBLES_EQUAL(25, temperature.temp, 45);
}

TEST(FsCheckout, CanSenseFsTorquerCurrentX) {
    CurrentReading current =
        measurable_manager->ReadNanopbMeasurable<CurrentReading>(kFsTorquerXI,
                                                                  0);
    DOUBLES_EQUAL(0.75, current.current, 0.76);
}

TEST(FsCheckout, CanSenseFsTorquerCurrentTotal) {
    CurrentReading current =
        measurable_manager->ReadNanopbMeasurable<CurrentReading>(
            kFsTorquerTotalI, 0);
    DOUBLES_EQUAL(2, current.current, 2.01);
}

TEST(FsCheckout, CanSenseFsTorquerCurrentY) {
    CurrentReading current =
        measurable_manager->ReadNanopbMeasurable<CurrentReading>(kFsTorquerYI,
                                                                  0);
    DOUBLES_EQUAL(0.75, current.current, 0.76);
}

TEST(FsCheckout, CanSenseFsTorquerCurrentZ) {
    CurrentReading current =
        measurable_manager->ReadNanopbMeasurable<CurrentReading>(kFsTorquerZI,
                                                                  0);
    DOUBLES_EQUAL(0.75, current.current, 0.76);
}
