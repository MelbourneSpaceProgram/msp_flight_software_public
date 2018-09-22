#include <CppUTest/TestHarness.h>
#include <src/config/unit_tests.h>
#include <src/messages/CurrentReading.pb.h>
#include <src/messages/TemperatureReading.pb.h>
#include <src/messages/VoltageReading.pb.h>
#include <src/sensors/i2c_measurable_manager.h>
#include <src/sensors/i2c_sensors/measurables/imu_temperature_measurable.h>

TEST_GROUP(FsCheckout) {
    void setup() {
        if (!kFsBoardAvailable) {
            TEST_EXIT
        }
    };
};

static I2cMeasurableManager* i2c_measurable_manager =
    I2cMeasurableManager::GetInstance();

TEST(FsCheckout, CanSenseHBridgeXTemperature) {
    TemperatureReading fs_hb_xt =
        i2c_measurable_manager->ReadI2cMeasurable<TemperatureReading>(kFsHbXT,
                                                                      0);
    DOUBLES_EQUAL(20, fs_hb_xt.temp, 45);
}

TEST(FsCheckout, CanSenseHBridgeYTemperature) {
    TemperatureReading fs_hb_yt =
        i2c_measurable_manager->ReadI2cMeasurable<TemperatureReading>(kFsHbYT,
                                                                      0);
    DOUBLES_EQUAL(20, fs_hb_yt.temp, 45);
}

TEST(FsCheckout, CanSenseHBridgeZTemperature) {
    TemperatureReading fs_hb_zt =
        i2c_measurable_manager->ReadI2cMeasurable<TemperatureReading>(kFsHbZT,
                                                                      0);
    DOUBLES_EQUAL(20, fs_hb_zt.temp, 45);
}

TEST(FsCheckout, CanSenseMagTorqXVoltage) {
    VoltageReading voltage_a =
        i2c_measurable_manager->ReadI2cMeasurable<VoltageReading>(kFsTorquerXAV,
                                                                  0);
    DOUBLES_EQUAL(2.5, voltage_a.voltage, 3);

    VoltageReading voltage_b =
        i2c_measurable_manager->ReadI2cMeasurable<VoltageReading>(kFsTorquerXBV,
                                                                  0);
    DOUBLES_EQUAL(2.5, voltage_b.voltage, 3);
}

TEST(FsCheckout, CanSenseMagTorqYVoltage) {
    VoltageReading voltage_a =
        i2c_measurable_manager->ReadI2cMeasurable<VoltageReading>(kFsTorquerYAV,
                                                                  0);
    DOUBLES_EQUAL(2.5, voltage_a.voltage, 3);

    VoltageReading voltage_b =
        i2c_measurable_manager->ReadI2cMeasurable<VoltageReading>(kFsTorquerYBV,
                                                                  0);
    DOUBLES_EQUAL(2.5, voltage_b.voltage, 3);
}

TEST(FsCheckout, CanSenseMagTorqZVoltage) {
    VoltageReading voltage_a =
        i2c_measurable_manager->ReadI2cMeasurable<VoltageReading>(kFsTorquerZAV,
                                                                  0);
    DOUBLES_EQUAL(2.5, voltage_a.voltage, 3);

    VoltageReading voltage_b =
        i2c_measurable_manager->ReadI2cMeasurable<VoltageReading>(kFsTorquerZBV,
                                                                  0);
    DOUBLES_EQUAL(2.5, voltage_b.voltage, 3);
}

TEST(FsCheckout, Imu1) {
    TemperatureReading temperature =
        i2c_measurable_manager->ReadI2cMeasurable<TemperatureReading>(kFsImuT1,
                                                                      0);

    DOUBLES_EQUAL(25, temperature.temp, 45);
}

TEST(FsCheckout, Imu2) {
    TemperatureReading temperature =
        i2c_measurable_manager->ReadI2cMeasurable<TemperatureReading>(kFsImuT2,
                                                                      0);

    DOUBLES_EQUAL(25, temperature.temp, 45);
}

TEST(FsCheckout, CanSenseFsTorquerCurrentX) {
    CurrentReading current =
        i2c_measurable_manager->ReadI2cMeasurable<CurrentReading>(kFsTorquerXI,
                                                                  0);
    DOUBLES_EQUAL(0.75, current.current, 0.76);
}

TEST(FsCheckout, CanSenseFsTorquerCurrentTotal) {
    CurrentReading current =
        i2c_measurable_manager->ReadI2cMeasurable<CurrentReading>(
            kFsTorquerTotalI, 0);
    DOUBLES_EQUAL(2, current.current, 2.01);
}

TEST(FsCheckout, CanSenseFsTorquerCurrentY) {
    CurrentReading current =
        i2c_measurable_manager->ReadI2cMeasurable<CurrentReading>(kFsTorquerYI,
                                                                  0);
    DOUBLES_EQUAL(0.75, current.current, 0.76);
}

TEST(FsCheckout, CanSenseFsTorquerCurrentZ) {
    CurrentReading current =
        i2c_measurable_manager->ReadI2cMeasurable<CurrentReading>(kFsTorquerZI,
                                                                  0);
    DOUBLES_EQUAL(0.75, current.current, 0.76);
}
