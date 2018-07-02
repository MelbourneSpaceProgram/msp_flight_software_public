#include <CppUTest/TestHarness.h>
#include <src/config/unit_tests.h>
#include <src/messages/MagnetometerReading.pb.h>
#include <src/sensors/specific_sensors/magnetometer.h>

TEST_GROUP(Magnetometer) {
    void setup() {
        if (!imu_available || !hil_available) {
            TEST_EXIT;
        }
    };
};

TEST(Magnetometer, TestMagnetometer) {
    Magnetometer magnetometer;
    bool success = magnetometer.TakeReading();

    CHECK(success);
}
