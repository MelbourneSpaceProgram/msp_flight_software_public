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

TEST(PanelCheckout, XPosTemp1) {
    double temp = i2c_measurable_manager->ReadI2cMeasurable<double>(kXPosT1, 0);
    DOUBLES_EQUAL(50, temp, 50);
}

TEST(PanelCheckout, XPosTemp2) {
    double temp = i2c_measurable_manager->ReadI2cMeasurable<double>(kXPosT2, 0);
    DOUBLES_EQUAL(50, temp, 50);
}

TEST(PanelCheckout, YPosTemp1) {
    TEST_EXIT;
    double temp = i2c_measurable_manager->ReadI2cMeasurable<double>(kYPosT1, 0);
    DOUBLES_EQUAL(50, temp, 50);
}

TEST(PanelCheckout, YPosTemp2) {
    TEST_EXIT;
    double temp = i2c_measurable_manager->ReadI2cMeasurable<double>(kYPosT2, 0);
    DOUBLES_EQUAL(50, temp, 50);
}

TEST(PanelCheckout, XNegTemp1) {
    double temp = i2c_measurable_manager->ReadI2cMeasurable<double>(kXNegT1, 0);
    DOUBLES_EQUAL(50, temp, 50);
}

TEST(PanelCheckout, XNegTemp2) {
    double temp = i2c_measurable_manager->ReadI2cMeasurable<double>(kXNegT2, 0);
    DOUBLES_EQUAL(50, temp, 50);
}

TEST(PanelCheckout, YNegTemp1) {
    double temp = i2c_measurable_manager->ReadI2cMeasurable<double>(kYNegT1, 0);
    DOUBLES_EQUAL(50, temp, 50);
}

TEST(PanelCheckout, YNegTemp2) {
    double temp = i2c_measurable_manager->ReadI2cMeasurable<double>(kYNegT2, 0);
    DOUBLES_EQUAL(50, temp, 50);
}

TEST(PanelCheckout, ZNegTemp1) {
    double temp = i2c_measurable_manager->ReadI2cMeasurable<double>(kZNegT1, 0);
    DOUBLES_EQUAL(50, temp, 50);
}

TEST(PanelCheckout, ZNegTemp2) {
    double temp = i2c_measurable_manager->ReadI2cMeasurable<double>(kZNegT2, 0);
    DOUBLES_EQUAL(50, temp, 50);
}

TEST(PanelCheckout, ZPosTemp1) {
    double temp = i2c_measurable_manager->ReadI2cMeasurable<double>(kZPosT, 0);
    DOUBLES_EQUAL(50, temp, 50);
}

TEST(PanelCheckout, XPosVoltage) {
    double voltage =
        i2c_measurable_manager->ReadI2cMeasurable<double>(kXPosV, 0);
    DOUBLES_EQUAL(2.5, voltage, 3);
}

TEST(PanelCheckout, XPosSolarVoltage) {
    double voltage =
        i2c_measurable_manager->ReadI2cMeasurable<double>(kXPosSolarV, 0);
    DOUBLES_EQUAL(2.5, voltage, 3);
}

TEST(PanelCheckout, YPosVoltage) {
    TEST_EXIT;
    double voltage =
        i2c_measurable_manager->ReadI2cMeasurable<double>(kYPosV, 0);
    DOUBLES_EQUAL(2.5, voltage, 3);
}

TEST(PanelCheckout, YPosSolarVoltage) {
    TEST_EXIT;
    double voltage =
        i2c_measurable_manager->ReadI2cMeasurable<double>(kYPosSolarV, 0);
    DOUBLES_EQUAL(2.5, voltage, 3);
}

TEST(PanelCheckout, XNegVoltage) {
    double voltage =
        i2c_measurable_manager->ReadI2cMeasurable<double>(kXNegV, 0);
    DOUBLES_EQUAL(2.5, voltage, 3);
}

TEST(PanelCheckout, XNegSolarVoltage) {
    double voltage =
        i2c_measurable_manager->ReadI2cMeasurable<double>(kXNegSolarV, 0);
    DOUBLES_EQUAL(2.5, voltage, 3);
}

TEST(PanelCheckout, YNegVoltage) {
    double voltage =
        i2c_measurable_manager->ReadI2cMeasurable<double>(kYNegV, 0);
    DOUBLES_EQUAL(2.5, voltage, 3);
}

TEST(PanelCheckout, YNegSolarVoltage) {
    double voltage =
        i2c_measurable_manager->ReadI2cMeasurable<double>(kYNegSolarV, 0);
    DOUBLES_EQUAL(2.5, voltage, 3);
}

TEST(PanelCheckout, ZNegVoltage) {
    double voltage =
        i2c_measurable_manager->ReadI2cMeasurable<double>(kZNegV, 0);
    DOUBLES_EQUAL(2.5, voltage, 3);
}

TEST(PanelCheckout, ZNegSolarVoltage) {
    double voltage =
        i2c_measurable_manager->ReadI2cMeasurable<double>(kZNegSolarV, 0);
    DOUBLES_EQUAL(2.5, voltage, 3);
}

TEST(PanelCheckout, ZPosVoltage) {
    double voltage =
        i2c_measurable_manager->ReadI2cMeasurable<double>(kEpsTopPanelV, 0);
    DOUBLES_EQUAL(2.5, voltage, 3);
}

TEST(PanelCheckout, ZPosSolarVoltage) {
    double voltage =
        i2c_measurable_manager->ReadI2cMeasurable<double>(kEpsTopSolarV, 0);
    DOUBLES_EQUAL(2.5, voltage, 3);
}

TEST(PanelCheckout, ZPosSolarCurrent) {
    double current =
        i2c_measurable_manager->ReadI2cMeasurable<double>(kEpsTopSolarI, 0);
    DOUBLES_EQUAL(0.25, current, 0.25);
}

TEST(PanelCheckout, ZPosCurrent) {
    double current =
        i2c_measurable_manager->ReadI2cMeasurable<double>(kEpsTopPanelI, 0);
    DOUBLES_EQUAL(0.25, current, 0.25);
}

TEST(PanelCheckout, XPosSolarCurrent) {
    double current =
        i2c_measurable_manager->ReadI2cMeasurable<double>(kXPosSolarI, 0);
    DOUBLES_EQUAL(0.25, current, 0.25);
}

TEST(PanelCheckout, XPosCurrent) {
    double current =
        i2c_measurable_manager->ReadI2cMeasurable<double>(kXPosI, 0);
    DOUBLES_EQUAL(0.25, current, 0.25);
}

TEST(PanelCheckout, YPosSolarCurrent) {
    TEST_EXIT;
    double current =
        i2c_measurable_manager->ReadI2cMeasurable<double>(kYPosSolarI, 0);
    DOUBLES_EQUAL(0.25, current, 0.25);
}

TEST(PanelCheckout, YPosCurrent) {
    TEST_EXIT;
    double current =
        i2c_measurable_manager->ReadI2cMeasurable<double>(kYPosI, 0);
    DOUBLES_EQUAL(0.25, current, 0.25);
}

TEST(PanelCheckout, XNegSolarCurrent) {
    double current =
        i2c_measurable_manager->ReadI2cMeasurable<double>(kXNegSolarI, 0);
    DOUBLES_EQUAL(0.25, current, 0.25);
}

TEST(PanelCheckout, XNegCurrent) {
    double current =
        i2c_measurable_manager->ReadI2cMeasurable<double>(kXNegI, 0);
    DOUBLES_EQUAL(0.25, current, 0.25);
}

TEST(PanelCheckout, YNegSolarCurrent) {
    double current =
        i2c_measurable_manager->ReadI2cMeasurable<double>(kYNegSolarI, 0);
    DOUBLES_EQUAL(0.25, current, 0.25);
}

TEST(PanelCheckout, YNegCurrent) {
    double current =
        i2c_measurable_manager->ReadI2cMeasurable<double>(kYNegI, 0);
    DOUBLES_EQUAL(0.25, current, 0.25);
}

TEST(PanelCheckout, ZNegSolarCurrent) {
    double current =
        i2c_measurable_manager->ReadI2cMeasurable<double>(kZNegSolarI, 0);
    DOUBLES_EQUAL(0.25, current, 0.25);
}

TEST(PanelCheckout, ZNegCurrent) {
    double current =
        i2c_measurable_manager->ReadI2cMeasurable<double>(kZNegI, 0);
    DOUBLES_EQUAL(0.25, current, 0.25);
}
