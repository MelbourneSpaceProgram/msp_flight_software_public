#include <CppUTest/TestHarness.h>
#include <src/config/unit_tests.h>
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
    double fs_hb_xt =
        i2c_measurable_manager->ReadI2cMeasurable<double>(kFsHbXT, 0);
    DOUBLES_EQUAL(50, fs_hb_xt, 50);
}

TEST(FsCheckout, CanSenseHBridgeYTemperature) {
    double fs_hb_yt =
        i2c_measurable_manager->ReadI2cMeasurable<double>(kFsHbYT, 0);
    DOUBLES_EQUAL(50, fs_hb_yt, 50);
}

TEST(FsCheckout, CanSenseHBridgeZTemperature) {
    double fs_hb_zt =
        i2c_measurable_manager->ReadI2cMeasurable<double>(kFsHbZT, 0);
    DOUBLES_EQUAL(50, fs_hb_zt, 50);
}

TEST(FsCheckout, CanSenseMagTorqXVoltage) {
    double voltage_a =
        i2c_measurable_manager->ReadI2cMeasurable<double>(kFsTorquerXAV, 0);
    DOUBLES_EQUAL(2.5, voltage_a, 3);

    double voltage_b =
        i2c_measurable_manager->ReadI2cMeasurable<double>(kFsTorquerXBV, 0);
    DOUBLES_EQUAL(2.5, voltage_b, 3);
}

TEST(FsCheckout, CanSenseMagTorqYVoltage) {
    double voltage_a =
        i2c_measurable_manager->ReadI2cMeasurable<double>(kFsTorquerYAV, 0);
    DOUBLES_EQUAL(2.5, voltage_a, 3);

    double voltage_b =
        i2c_measurable_manager->ReadI2cMeasurable<double>(kFsTorquerYBV, 0);
    DOUBLES_EQUAL(2.5, voltage_b, 3);
}

TEST(FsCheckout, CanSenseMagTorqZVoltage) {
    double voltage_a =
        i2c_measurable_manager->ReadI2cMeasurable<double>(kFsTorquerZAV, 0);
    DOUBLES_EQUAL(2.5, voltage_a, 3);

    double voltage_b =
        i2c_measurable_manager->ReadI2cMeasurable<double>(kFsTorquerZBV, 0);
    DOUBLES_EQUAL(2.5, voltage_b, 3);
}

TEST(FsCheckout, Imu1) {
    double temperature = i2c_measurable_manager->ReadI2cMeasurable<double>(
        kFsImuT1, 0);

    DOUBLES_EQUAL(25, temperature, 10);
}

TEST(FsCheckout, Imu2) {
    double temperature = i2c_measurable_manager->ReadI2cMeasurable<double>(
        kFsImuT2, 0);

    DOUBLES_EQUAL(25, temperature, 10);
}

TEST(FsCheckout, CanSenseFsTorquerCurrentX) {
    double current = i2c_measurable_manager->ReadI2cMeasurable<double>(
        kFsTorquerXI, 0);
    DOUBLES_EQUAL(0.75, current, 0.76);
}

TEST(FsCheckout, CanSenseFsTorquerCurrentTotal) {
    double current = i2c_measurable_manager->ReadI2cMeasurable<double>(
        kFsTorquerTotalI, 0);
    DOUBLES_EQUAL(2, current, 2.01);
}

TEST(FsCheckout, CanSenseFsTorquerCurrentY) {
    double current = i2c_measurable_manager->ReadI2cMeasurable<double>(
        kFsTorquerYI, 0);
    DOUBLES_EQUAL(0.75, current, 0.76);
}

TEST(FsCheckout, CanSenseFsTorquerCurrentZ) {
    double current = i2c_measurable_manager->ReadI2cMeasurable<double>(
        kFsTorquerZI, 0);
    DOUBLES_EQUAL(0.75, current, 0.76);
}
