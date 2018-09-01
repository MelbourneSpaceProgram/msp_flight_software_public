#include <CppUTest/TestHarness.h>
#include <src/config/unit_tests.h>
#include <src/sensors/i2c_measurable_manager.h>
#include <src/sensors/i2c_sensors/measurables/imu_temperature_measurable.h>

TEST_GROUP(PanelCheckout) {
    void setup() {
        if (!kPanelBoardAvailable) {
            TEST_EXIT
        }
    };
};

static I2cMeasurableManager* i2c_measurable_manager =
    I2cMeasurableManager::GetInstance();

TEST(PanelCheckout, CanSensePowerPanel1Temp1) {
    double temp =
        i2c_measurable_manager->ReadI2cMeasurable<double>(kXPosT1, 0);
    DOUBLES_EQUAL(50, temp, 50);
}

TEST(PanelCheckout, CanSensePowerPanel1Temp2) {
    double temp =
        i2c_measurable_manager->ReadI2cMeasurable<double>(kXPosT2, 0);
    DOUBLES_EQUAL(50, temp, 50);
}

TEST(PanelCheckout, CanSensePowerPanel2Temp1) {
    double temp =
        i2c_measurable_manager->ReadI2cMeasurable<double>(kYPosT1, 0);
    DOUBLES_EQUAL(50, temp, 50);
}

TEST(PanelCheckout, CanSensePowerPanel2Temp2) {
    double temp =
        i2c_measurable_manager->ReadI2cMeasurable<double>(kYPosT2, 0);
    DOUBLES_EQUAL(50, temp, 50);
}

TEST(PanelCheckout, CanSensePowerPanel3Temp1) {
    double temp =
        i2c_measurable_manager->ReadI2cMeasurable<double>(kXNegT1, 0);
    DOUBLES_EQUAL(50, temp, 50);
}

TEST(PanelCheckout, CanSensePowerPanel3Temp2) {
    double temp =
        i2c_measurable_manager->ReadI2cMeasurable<double>(kXNegT2, 0);
    DOUBLES_EQUAL(50, temp, 50);
}

TEST(PanelCheckout, CanSensePowerPanel4Temp1) {
    double temp =
        i2c_measurable_manager->ReadI2cMeasurable<double>(kYNegT1, 0);
    DOUBLES_EQUAL(50, temp, 50);
}

TEST(PanelCheckout, CanSensePowerPanel4Temp2) {
    double temp =
        i2c_measurable_manager->ReadI2cMeasurable<double>(kYNegT2, 0);
    DOUBLES_EQUAL(50, temp, 50);
}

TEST(PanelCheckout, CanSensePowerPanel5Temp1) {
    double temp =
        i2c_measurable_manager->ReadI2cMeasurable<double>(kZNegT1, 0);
    DOUBLES_EQUAL(50, temp, 50);
}

TEST(PanelCheckout, CanSensePowerPanel5Temp2) {
    double temp =
        i2c_measurable_manager->ReadI2cMeasurable<double>(kZNegT2, 0);
    DOUBLES_EQUAL(50, temp, 50);
}

TEST(PanelCheckout, CanSensePowerPanel6Temp1) {
    double temp =
        i2c_measurable_manager->ReadI2cMeasurable<double>(kZPosT, 0);
    DOUBLES_EQUAL(50, temp, 50);
}

TEST(PanelCheckout, CanSensePowerPanelVoltage1) {
    double voltage = i2c_measurable_manager->ReadI2cMeasurable<double>(
        kXPosV, 0);
    DOUBLES_EQUAL(2.5, voltage, 3);
}

TEST(PanelCheckout, CanSensePowerSolarVoltage1) {
    double voltage = i2c_measurable_manager->ReadI2cMeasurable<double>(
        kXPosSolarV, 0);
    DOUBLES_EQUAL(2.5, voltage, 3);
}

TEST(PanelCheckout, CanSensePowerPanelVoltage2) {
    double voltage = i2c_measurable_manager->ReadI2cMeasurable<double>(
        kYPosV, 0);
    DOUBLES_EQUAL(2.5, voltage, 3);
}

TEST(PanelCheckout, CanSensePowerSolarVoltage2) {
    double voltage = i2c_measurable_manager->ReadI2cMeasurable<double>(
        kYPosSolarV, 0);
    DOUBLES_EQUAL(2.5, voltage, 3);
}

TEST(PanelCheckout, CanSensePowerPanelVoltage3) {
    double voltage = i2c_measurable_manager->ReadI2cMeasurable<double>(
        kXNegV, 0);
    DOUBLES_EQUAL(2.5, voltage, 3);
}

TEST(PanelCheckout, CanSensePowerSolarVoltage3) {
    double voltage = i2c_measurable_manager->ReadI2cMeasurable<double>(
        kXNegSolarV, 0);
    DOUBLES_EQUAL(2.5, voltage, 3);
}

TEST(PanelCheckout, CanSensePowerPanelVoltage4) {
    double voltage = i2c_measurable_manager->ReadI2cMeasurable<double>(
        kYNegV, 0);
    DOUBLES_EQUAL(2.5, voltage, 3);
}

TEST(PanelCheckout, CanSensePowerSolarVoltage4) {
    double voltage = i2c_measurable_manager->ReadI2cMeasurable<double>(
        kYNegSolarV, 0);
    DOUBLES_EQUAL(2.5, voltage, 3);
}

TEST(PanelCheckout, CanSensePowerPanelVoltage5) {
    double voltage = i2c_measurable_manager->ReadI2cMeasurable<double>(
        kZNegV, 0);
    DOUBLES_EQUAL(2.5, voltage, 3);
}

TEST(PanelCheckout, CanSensePowerSolarVoltage5) {
    double voltage = i2c_measurable_manager->ReadI2cMeasurable<double>(
        kZNegSolarV, 0);
    DOUBLES_EQUAL(2.5, voltage, 3);
}

TEST(PanelCheckout, CanSensePowerTopPanelVoltage) {
    double voltage = i2c_measurable_manager->ReadI2cMeasurable<double>(
        kEpsTopPanelV, 0);
    DOUBLES_EQUAL(2.5, voltage, 3);
}

TEST(PanelCheckout, CanSensePowerTopSolarVoltage) {
    double voltage = i2c_measurable_manager->ReadI2cMeasurable<double>(
        kEpsTopSolarV, 0);
    DOUBLES_EQUAL(2.5, voltage, 3);
}

TEST(PanelCheckout, CanSensePowerTopSolarCurrent) {
    double current = i2c_measurable_manager->ReadI2cMeasurable<double>(
        kEpsTopSolarI, 0);
    DOUBLES_EQUAL(0.25, current, 0.25);
}

TEST(PanelCheckout, CanSensePowerTopPanelCurrent) {
    double current = i2c_measurable_manager->ReadI2cMeasurable<double>(
        kEpsTopPanelI, 0);
    DOUBLES_EQUAL(0.25, current, 0.25);
}

TEST(PanelCheckout, CanSensePowerSolarCurrent1) {
    double current = i2c_measurable_manager->ReadI2cMeasurable<double>(
        kXPosSolarI, 0);
    DOUBLES_EQUAL(0.25, current, 0.25);
}

TEST(PanelCheckout, CanSensePowerPanelCurrent1) {
    double current = i2c_measurable_manager->ReadI2cMeasurable<double>(
        kXPosI, 0);
    DOUBLES_EQUAL(0.25, current, 0.25);
}

TEST(PanelCheckout, CanSensePowerSolarCurrent2) {
    double current = i2c_measurable_manager->ReadI2cMeasurable<double>(
        kYPosSolarI, 0);
    DOUBLES_EQUAL(0.25, current, 0.25);
}

TEST(PanelCheckout, CanSensePowerPanelCurrent2) {
    double current = i2c_measurable_manager->ReadI2cMeasurable<double>(
        kYPosI, 0);
    DOUBLES_EQUAL(0.25, current, 0.25);
}

TEST(PanelCheckout, CanSensePowerSolarCurrent3) {
    double current = i2c_measurable_manager->ReadI2cMeasurable<double>(
        kXNegSolarI, 0);
    DOUBLES_EQUAL(0.25, current, 0.25);
}

TEST(PanelCheckout, CanSensePowerPanelCurrent3) {
    double current = i2c_measurable_manager->ReadI2cMeasurable<double>(
        kXNegI, 0);
    DOUBLES_EQUAL(0.25, current, 0.25);
}

TEST(PanelCheckout, CanSensePowerSolarCurrent4) {
    double current = i2c_measurable_manager->ReadI2cMeasurable<double>(
        kYNegSolarI, 0);
    DOUBLES_EQUAL(0.25, current, 0.25);
}

TEST(PanelCheckout, CanSensePowerPanelCurrent4) {
    double current = i2c_measurable_manager->ReadI2cMeasurable<double>(
        kYNegI, 0);
    DOUBLES_EQUAL(0.25, current, 0.25);
}

TEST(PanelCheckout, CanSensePowerSolarCurrent5) {
    double current = i2c_measurable_manager->ReadI2cMeasurable<double>(
        kZNegSolarI, 0);
    DOUBLES_EQUAL(0.25, current, 0.25);
}

TEST(PanelCheckout, CanSensePowerPanelCurrent5) {
    double current = i2c_measurable_manager->ReadI2cMeasurable<double>(
        kZNegI, 0);
    DOUBLES_EQUAL(0.25, current, 0.25);
}
