#include <src/messages/MagnetometerReading.pb.h>
#include <src/sensors/specific_sensors/magnetometer.h>
#include <test_runners/unity.h>
#include <src/config/unit_tests.h>

void TestMagnetometer() {
    if (!imu_test_enabled || !hil_enabled) {
        TEST_IGNORE_MESSAGE("Ignore Magnetometer tests without HIL or I2C");
    }
    Magnetometer magnetometer;
    bool success = magnetometer.TakeReading();

    TEST_ASSERT_TRUE(success);
}
