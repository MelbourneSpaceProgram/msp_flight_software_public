#include <CppUTest/TestHarness.h>
#include <src/config/unit_tests.h>
#include <src/sensors/i2c_measurable_manager.h>
#include <src/sensors/i2c_sensors/measurables/imu_temperature_measurable.h>

TEST_GROUP(PanelCheckout) {
    void setup() {
        if (!panel_board_available) {
            TEST_EXIT
        }
    };
};

static I2cMeasurableManager* i2c_measurable_manager =
    I2cMeasurableManager::GetInstance();

TEST(PanelCheckout, CanSensePowerPanel1Temp1) {
    double temp =
        i2c_measurable_manager->ReadI2cMeasurable<double>(kPowerPanel1Temp1, 0);
    DOUBLES_EQUAL(25, temp, 10);
}

TEST(PanelCheckout, CanSensePowerPanel1Temp2) {
    double temp =
        i2c_measurable_manager->ReadI2cMeasurable<double>(kPowerPanel1Temp2, 0);
    DOUBLES_EQUAL(25, temp, 10);
}

TEST(PanelCheckout, CanSensePowerPanel2Temp1) {
    double temp =
        i2c_measurable_manager->ReadI2cMeasurable<double>(kPowerPanel2Temp1, 0);
    DOUBLES_EQUAL(25, temp, 10);
}

TEST(PanelCheckout, CanSensePowerPanel2Temp2) {
    double temp =
        i2c_measurable_manager->ReadI2cMeasurable<double>(kPowerPanel2Temp2, 0);
    DOUBLES_EQUAL(25, temp, 10);
}

TEST(PanelCheckout, CanSensePowerPanel3Temp1) {
    double temp =
        i2c_measurable_manager->ReadI2cMeasurable<double>(kPowerPanel3Temp1, 0);
    DOUBLES_EQUAL(25, temp, 10);
}

TEST(PanelCheckout, CanSensePowerPanel3Temp2) {
    double temp =
        i2c_measurable_manager->ReadI2cMeasurable<double>(kPowerPanel3Temp2, 0);
    DOUBLES_EQUAL(25, temp, 10);
}

TEST(PanelCheckout, CanSensePowerPanel4Temp1) {
    double temp =
        i2c_measurable_manager->ReadI2cMeasurable<double>(kPowerPanel4Temp1, 0);
    DOUBLES_EQUAL(25, temp, 10);
}

TEST(PanelCheckout, CanSensePowerPanel4Temp2) {
    double temp =
        i2c_measurable_manager->ReadI2cMeasurable<double>(kPowerPanel4Temp2, 0);
    DOUBLES_EQUAL(25, temp, 10);
}

TEST(PanelCheckout, CanSensePowerPanel5Temp1) {
    double temp =
        i2c_measurable_manager->ReadI2cMeasurable<double>(kPowerPanel5Temp1, 0);
    DOUBLES_EQUAL(25, temp, 10);
}

TEST(PanelCheckout, CanSensePowerPanel5Temp2) {
    double temp =
        i2c_measurable_manager->ReadI2cMeasurable<double>(kPowerPanel5Temp2, 0);
    DOUBLES_EQUAL(25, temp, 10);
}

TEST(PanelCheckout, CanSensePowerPanel6Temp1) {
    double temp =
        i2c_measurable_manager->ReadI2cMeasurable<double>(kPowerPanel6Temp1, 0);
    DOUBLES_EQUAL(25, temp, 10);
}
