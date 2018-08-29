#include <CppUTest/TestHarness.h>
#include <math.h>
#include <src/adcs/magnetorquer_control.h>
#include <src/config/unit_tests.h>
#include <src/sensors/i2c_measurable_manager.h>
#include <src/sensors/measurable_id.h>
#include <src/util/task_utils.h>

TEST_GROUP(Magnetorquer){};

TEST(Magnetorquer, MagnetorquerConnectivity) {
    if (!i2c_available || !fs_board_available || !utilities_board_available) {
        TEST_EXIT;
    }

    // Drive magnetorquers at full power.
    MagnetorquerControl::SetMagnetorquersPowerFraction(1, 1, 1);

    // Wait a few milliseconds for the current to rise through the magnetorquers
    // (they are basically big inductors, slowing changes in current).
    TaskUtils::SleepMilli(MagnetorquerControl::kDegaussingSwitchPeriodMicros /
                          1000);

    // Use current sensors to demonstrate that current is flowing through
    // the magnetorquers. If not, there is a driver problem or a hardware
    // connectivity problem.
    I2cMeasurableManager* measurable_manager =
        I2cMeasurableManager::GetInstance();
    double current_reading_x =
        measurable_manager->ReadI2cMeasurable<double>(kFsTorquerXI, 0);
    double current_reading_y =
        measurable_manager->ReadI2cMeasurable<double>(kFsTorquerYI, 0);
    double current_reading_z =
        measurable_manager->ReadI2cMeasurable<double>(kFsTorquerZI, 0);

    // Degauss magnetorquers so that they don't interfere with other magnetic
    // tests.
    MagnetorquerControl::Degauss();

    // The current through the magnetorquers at full power should be
    // at around +/-100mA, however any non-zero reading shows that they are
    // connected.
    DOUBLES_EQUAL(0.100, fabs(current_reading_x), 0.08);
    DOUBLES_EQUAL(0.100, fabs(current_reading_y), 0.08);
    DOUBLES_EQUAL(0.100, fabs(current_reading_z), 0.08);
}
