#include <CppUTest/TestHarness.h>
#include <src/config/unit_tests.h>
#include <src/sensors/i2c_measurable_manager.h>

TEST_GROUP(CdhCheckout) {
    void setup() {
        if (!cdh_board_available) {
            TEST_EXIT
        }
    };
};

static I2cMeasurableManager* i2c_measurable_manager =
    I2cMeasurableManager::GetInstance();

TEST(CdhCheckout, CanSenseSysTemperature) {
    double cdh_sys_temp =
        i2c_measurable_manager->ReadI2cMeasurable<double>(kCdhSysTemp, 0);
    DOUBLES_EQUAL(20, cdh_sys_temp, 5);
}
