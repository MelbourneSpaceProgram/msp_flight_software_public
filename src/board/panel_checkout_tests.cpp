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
    DOUBLES_EQUAL(50, temp, 50);
}

TEST(PanelCheckout, CanSensePowerPanel1Temp2) {
    double temp =
        i2c_measurable_manager->ReadI2cMeasurable<double>(kPowerPanel1Temp2, 0);
    DOUBLES_EQUAL(50, temp, 50);
}

TEST(PanelCheckout, CanSensePowerPanel2Temp1) {
    double temp =
        i2c_measurable_manager->ReadI2cMeasurable<double>(kPowerPanel2Temp1, 0);
    DOUBLES_EQUAL(50, temp, 50);
}

TEST(PanelCheckout, CanSensePowerPanel2Temp2) {
    double temp =
        i2c_measurable_manager->ReadI2cMeasurable<double>(kPowerPanel2Temp2, 0);
    DOUBLES_EQUAL(50, temp, 50);
}

TEST(PanelCheckout, CanSensePowerPanel3Temp1) {
    double temp =
        i2c_measurable_manager->ReadI2cMeasurable<double>(kPowerPanel3Temp1, 0);
    DOUBLES_EQUAL(50, temp, 50);
}

TEST(PanelCheckout, CanSensePowerPanel3Temp2) {
    double temp =
        i2c_measurable_manager->ReadI2cMeasurable<double>(kPowerPanel3Temp2, 0);
    DOUBLES_EQUAL(50, temp, 50);
}

TEST(PanelCheckout, CanSensePowerPanel4Temp1) {
    double temp =
        i2c_measurable_manager->ReadI2cMeasurable<double>(kPowerPanel4Temp1, 0);
    DOUBLES_EQUAL(50, temp, 50);
}

TEST(PanelCheckout, CanSensePowerPanel4Temp2) {
    double temp =
        i2c_measurable_manager->ReadI2cMeasurable<double>(kPowerPanel4Temp2, 0);
    DOUBLES_EQUAL(50, temp, 50);
}

TEST(PanelCheckout, CanSensePowerPanel5Temp1) {
    double temp =
        i2c_measurable_manager->ReadI2cMeasurable<double>(kPowerPanel5Temp1, 0);
    DOUBLES_EQUAL(50, temp, 50);
}

TEST(PanelCheckout, CanSensePowerPanel5Temp2) {
    double temp =
        i2c_measurable_manager->ReadI2cMeasurable<double>(kPowerPanel5Temp2, 0);
    DOUBLES_EQUAL(50, temp, 50);
}

TEST(PanelCheckout, CanSensePowerPanel6Temp1) {
    double temp =
        i2c_measurable_manager->ReadI2cMeasurable<double>(kPowerPanel6Temp1, 0);
    DOUBLES_EQUAL(50, temp, 50);
}

TEST(PanelCheckout, CanSensePowerPanelVoltage1) {
    double voltage = i2c_measurable_manager->ReadI2cMeasurable<double>(
        kPowerPanelVoltage1, 0);
    DOUBLES_EQUAL(2.5, voltage, 3);
}

TEST(PanelCheckout, CanSensePowerSolarVoltage1) {
    double voltage = i2c_measurable_manager->ReadI2cMeasurable<double>(
        kPowerSolarVoltage1, 0);
    DOUBLES_EQUAL(2.5, voltage, 3);
}

TEST(PanelCheckout, CanSensePowerPanelVoltage2) {
    double voltage = i2c_measurable_manager->ReadI2cMeasurable<double>(
        kPowerPanelVoltage2, 0);
    DOUBLES_EQUAL(2.5, voltage, 3);
}

TEST(PanelCheckout, CanSensePowerSolarVoltage2) {
    double voltage = i2c_measurable_manager->ReadI2cMeasurable<double>(
        kPowerSolarVoltage2, 0);
    DOUBLES_EQUAL(2.5, voltage, 3);
}

TEST(PanelCheckout, CanSensePowerPanelVoltage3) {
    double voltage = i2c_measurable_manager->ReadI2cMeasurable<double>(
        kPowerPanelVoltage3, 0);
    DOUBLES_EQUAL(2.5, voltage, 3);
}

TEST(PanelCheckout, CanSensePowerSolarVoltage3) {
    double voltage = i2c_measurable_manager->ReadI2cMeasurable<double>(
        kPowerSolarVoltage3, 0);
    DOUBLES_EQUAL(2.5, voltage, 3);
}

TEST(PanelCheckout, CanSensePowerPanelVoltage4) {
    double voltage = i2c_measurable_manager->ReadI2cMeasurable<double>(
        kPowerPanelVoltage4, 0);
    DOUBLES_EQUAL(2.5, voltage, 3);
}

TEST(PanelCheckout, CanSensePowerSolarVoltage4) {
    double voltage = i2c_measurable_manager->ReadI2cMeasurable<double>(
        kPowerSolarVoltage4, 0);
    DOUBLES_EQUAL(2.5, voltage, 3);
}

TEST(PanelCheckout, CanSensePowerPanelVoltage5) {
    double voltage = i2c_measurable_manager->ReadI2cMeasurable<double>(
        kPowerPanelVoltage5, 0);
    DOUBLES_EQUAL(2.5, voltage, 3);
}

TEST(PanelCheckout, CanSensePowerSolarVoltage5) {
    double voltage = i2c_measurable_manager->ReadI2cMeasurable<double>(
        kPowerSolarVoltage5, 0);
    DOUBLES_EQUAL(2.5, voltage, 3);
}

TEST(PanelCheckout, CanSensePowerTopPanelVoltage) {
    double voltage = i2c_measurable_manager->ReadI2cMeasurable<double>(
        kPowerTopPanelVoltage, 0);
    DOUBLES_EQUAL(2.5, voltage, 3);
}

TEST(PanelCheckout, CanSensePowerTopSolarVoltage) {
    double voltage = i2c_measurable_manager->ReadI2cMeasurable<double>(
        kPowerTopSolarVoltage, 0);
    DOUBLES_EQUAL(2.5, voltage, 3);
}

TEST(PanelCheckout, CanSensePowerTopSolarCurrent) {
    double current = i2c_measurable_manager->ReadI2cMeasurable<double>(
        kPowerTopSolarCurrent, 0);
    DOUBLES_EQUAL(0.25, current, 0.25);
}

TEST(PanelCheckout, CanSensePowerTopPanelCurrent) {
    double current = i2c_measurable_manager->ReadI2cMeasurable<double>(
        kPowerTopPanelCurrent, 0);
    DOUBLES_EQUAL(0.25, current, 0.25);
}

TEST(PanelCheckout, CanSensePowerSolarCurrent1) {
    double current = i2c_measurable_manager->ReadI2cMeasurable<double>(
        kPowerSolarCurrent1, 0);
    DOUBLES_EQUAL(0.25, current, 0.25);
}

TEST(PanelCheckout, CanSensePowerPanelCurrent1) {
    double current = i2c_measurable_manager->ReadI2cMeasurable<double>(
        kPowerPanelCurrent1, 0);
    DOUBLES_EQUAL(0.25, current, 0.25);
}

TEST(PanelCheckout, CanSensePowerSolarCurrent2) {
    double current = i2c_measurable_manager->ReadI2cMeasurable<double>(
        kPowerSolarCurrent2, 0);
    DOUBLES_EQUAL(0.25, current, 0.25);
}

TEST(PanelCheckout, CanSensePowerPanelCurrent2) {
    double current = i2c_measurable_manager->ReadI2cMeasurable<double>(
        kPowerPanelCurrent2, 0);
    DOUBLES_EQUAL(0.25, current, 0.25);
}

TEST(PanelCheckout, CanSensePowerSolarCurrent3) {
    double current = i2c_measurable_manager->ReadI2cMeasurable<double>(
        kPowerSolarCurrent3, 0);
    DOUBLES_EQUAL(0.25, current, 0.25);
}

TEST(PanelCheckout, CanSensePowerPanelCurrent3) {
    double current = i2c_measurable_manager->ReadI2cMeasurable<double>(
        kPowerPanelCurrent3, 0);
    DOUBLES_EQUAL(0.25, current, 0.25);
}

TEST(PanelCheckout, CanSensePowerSolarCurrent4) {
    double current = i2c_measurable_manager->ReadI2cMeasurable<double>(
        kPowerSolarCurrent4, 0);
    DOUBLES_EQUAL(0.25, current, 0.25);
}

TEST(PanelCheckout, CanSensePowerPanelCurrent4) {
    double current = i2c_measurable_manager->ReadI2cMeasurable<double>(
        kPowerPanelCurrent4, 0);
    DOUBLES_EQUAL(0.25, current, 0.25);
}

TEST(PanelCheckout, CanSensePowerSolarCurrent5) {
    double current = i2c_measurable_manager->ReadI2cMeasurable<double>(
        kPowerSolarCurrent5, 0);
    DOUBLES_EQUAL(0.25, current, 0.25);
}

TEST(PanelCheckout, CanSensePowerPanelCurrent5) {
    double current = i2c_measurable_manager->ReadI2cMeasurable<double>(
        kPowerPanelCurrent5, 0);
    DOUBLES_EQUAL(0.25, current, 0.25);
}
