#include <CppUTest/TestHarness.h>
#include <src/config/unit_tests.h>
#include <src/sensors/i2c_measurable_manager.h>
#include <src/sensors/i2c_sensors/measurables/imu_temperature_measurable.h>

TEST_GROUP(FsCheckout) {
    void setup() {
        if (!fs_board_available) {
            TEST_EXIT
        }
    };
};

static I2cMeasurableManager* i2c_measurable_manager =
    I2cMeasurableManager::GetInstance();

TEST(FsCheckout, CanSenseHBridgeXTemperature) {
    double fs_temp_hb_x =
        i2c_measurable_manager->ReadI2cMeasurable<double>(kFsTempHbX, 0);
    DOUBLES_EQUAL(50, fs_temp_hb_x, 50);
}

TEST(FsCheckout, CanSenseHBridgeYTemperature) {
    double fs_temp_hb_y =
        i2c_measurable_manager->ReadI2cMeasurable<double>(kFsTempHbY, 0);
    DOUBLES_EQUAL(50, fs_temp_hb_y, 50);
}

TEST(FsCheckout, CanSenseHBridgeZTemperature) {
    double fs_temp_hb_z =
        i2c_measurable_manager->ReadI2cMeasurable<double>(kFsTempHbZ, 0);
    DOUBLES_EQUAL(50, fs_temp_hb_z, 50);
}

TEST(FsCheckout, CanSenseMagTorqXVoltage) {
    double voltage_a =
        i2c_measurable_manager->ReadI2cMeasurable<double>(kFsMagTorqAX, 0);
    DOUBLES_EQUAL(2.5, voltage_a, 3);

    double voltage_b =
        i2c_measurable_manager->ReadI2cMeasurable<double>(kFsMagTorqBX, 0);
    DOUBLES_EQUAL(2.5, voltage_b, 3);
}

TEST(FsCheckout, CanSenseMagTorqYVoltage) {
    double voltage_a =
        i2c_measurable_manager->ReadI2cMeasurable<double>(kFsMagTorqAY, 0);
    DOUBLES_EQUAL(2.5, voltage_a, 3);

    double voltage_b =
        i2c_measurable_manager->ReadI2cMeasurable<double>(kFsMagTorqBY, 0);
    DOUBLES_EQUAL(2.5, voltage_b, 3);
}

TEST(FsCheckout, CanSenseMagTorqZVoltage) {
    double voltage_a =
        i2c_measurable_manager->ReadI2cMeasurable<double>(kFsMagTorqAZ, 0);
    DOUBLES_EQUAL(2.5, voltage_a, 3);

    double voltage_b =
        i2c_measurable_manager->ReadI2cMeasurable<double>(kFsMagTorqBZ, 0);
    DOUBLES_EQUAL(2.5, voltage_b, 3);
}

TEST(FsCheckout, Imu1) {
    double temperature = i2c_measurable_manager->ReadI2cMeasurable<double>(
        kFsImuTemperature1, 0);

    DOUBLES_EQUAL(25, temperature, 10);
}

TEST(FsCheckout, Imu2) {
    double temperature = i2c_measurable_manager->ReadI2cMeasurable<double>(
        kFsImuTemperature2, 0);

    DOUBLES_EQUAL(25, temperature, 10);
}

TEST(FsCheckout, CanSenseFsTorquerCurrentX) {
    double current = i2c_measurable_manager->ReadI2cMeasurable<double>(
        kFsTorquerCurrentX, 0);
    DOUBLES_EQUAL(0.75, current, 0.76);
}

TEST(FsCheckout, CanSenseFsTorquerCurrentTotal) {
    double current = i2c_measurable_manager->ReadI2cMeasurable<double>(
        kFsTorquerCurrentTotal, 0);
    DOUBLES_EQUAL(2, current, 2.01);
}

TEST(FsCheckout, CanSenseFsTorquerCurrentY) {
    double current = i2c_measurable_manager->ReadI2cMeasurable<double>(
        kFsTorquerCurrentY, 0);
    DOUBLES_EQUAL(0.75, current, 0.76);
}

TEST(FsCheckout, CanSenseFsTorquerCurrentZ) {
    double current = i2c_measurable_manager->ReadI2cMeasurable<double>(
        kFsTorquerCurrentZ, 0);
    DOUBLES_EQUAL(0.75, current, 0.76);
}
