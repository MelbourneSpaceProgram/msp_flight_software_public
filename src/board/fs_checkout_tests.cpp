#include <CppUTest/TestHarness.h>
#include <src/config/unit_tests.h>
#include <src/sensors/i2c_measurable_manager.h>
#include <src/sensors/i2c_sensors/measurables/imu_temperature_measurable.h>

TEST_GROUP(FsCheckout){};
static I2cMeasurableManager* i2c_measurable_manager = I2cMeasurableManager::GetInstance();

TEST(FsCheckout, CanSenseHBridgeXTemperature) {
    double fs_temp_hb_x = i2c_measurable_manager->ReadI2cMeasurable<double>(kFsTempHbX, 0);
    DOUBLES_EQUAL(20, fs_temp_hb_x, 5);
}

TEST(FsCheckout, CanSenseHBridgeYTemperature) {
    double fs_temp_hb_y = i2c_measurable_manager->ReadI2cMeasurable<double>(kFsTempHbY, 0);
    DOUBLES_EQUAL(20, fs_temp_hb_y, 5);
}

TEST(FsCheckout, CanSenseHBridgeZTemperature) {
    double fs_temp_hb_z = i2c_measurable_manager->ReadI2cMeasurable<double>(kFsTempHbZ, 0);
    DOUBLES_EQUAL(20, fs_temp_hb_z, 5);
}

TEST(FsCheckout, Imu1) {
    Mpu9250TemperatureReading temperature = i2c_measurable_manager->ReadI2cMeasurable<Mpu9250TemperatureReading>(kFsImuTemperature1, 0);

    DOUBLES_EQUAL(20, temperature.temp, 5);
}

TEST(FsCheckout, Imu2) {
    Mpu9250TemperatureReading temperature = i2c_measurable_manager->ReadI2cMeasurable<Mpu9250TemperatureReading>(kFsImuTemperature2, 0);

    DOUBLES_EQUAL(20, temperature.temp, 5);
}

