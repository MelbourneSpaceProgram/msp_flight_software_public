#include <CppUTest/TestHarness.h>
#include <src/config/unit_tests.h>
#include <src/sensors/i2c_measurable_manager.h>
#include <src/sensors/i2c_sensors/measurables/imu_temperature_measurable.h>

TEST_GROUP(EpsCheckout) {
    void setup() {
        if (!eps_board_available) {
            TEST_EXIT
        }
    };
};

static I2cMeasurableManager* i2c_measurable_manager =
    I2cMeasurableManager::GetInstance();

TEST(EpsCheckout, CanSensePowerTemp1) {
    double temp =
        i2c_measurable_manager->ReadI2cMeasurable<double>(kPowerTemp1, 0);
    DOUBLES_EQUAL(25, temp, 5);
}

TEST(EpsCheckout, CanSensePowerTemp2) {
    double temp =
        i2c_measurable_manager->ReadI2cMeasurable<double>(kPowerTemp2, 0);
    DOUBLES_EQUAL(25, temp, 5);
}

TEST(EpsCheckout, CanSensePowerBmsDieTemp1) {
    double temp =
        i2c_measurable_manager->ReadI2cMeasurable<double>(kPowerBmsDieTemp1, 0);
    DOUBLES_EQUAL(50, temp, 30);
}

TEST(EpsCheckout, CanSensePowerBmsDieTemp2) {
    double temp =
        i2c_measurable_manager->ReadI2cMeasurable<double>(kPowerBmsDieTemp2, 0);
    DOUBLES_EQUAL(50, temp, 30);
}
