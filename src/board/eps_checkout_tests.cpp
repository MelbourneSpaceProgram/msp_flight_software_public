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

TEST(EpsCheckout, CanSenseEpsBatVoltage1) {
    double vbat =
        i2c_measurable_manager->ReadI2cMeasurable<double>(kEpsBatVoltage1, 0);
    DOUBLES_EQUAL(2.9, vbat, 0.9);
}

TEST(EpsCheckout, CanSenseEpsBatVoltage2) {
    double vbat =
        i2c_measurable_manager->ReadI2cMeasurable<double>(kEpsBatVoltage2, 0);
    DOUBLES_EQUAL(2.9, vbat, 0.9);
}

TEST(EpsCheckout, CanSenseEpsBoostOutVoltage1) {
    double boost = i2c_measurable_manager->ReadI2cMeasurable<double>(
        kEpsBoostOutVoltage1, 0);
    DOUBLES_EQUAL(5.36, boost, 0.1);
}

TEST(EpsCheckout, CanSenseEpsBoostOutVoltage2) {
    double boost = i2c_measurable_manager->ReadI2cMeasurable<double>(
        kEpsBoostOutVoltage2, 0);
    DOUBLES_EQUAL(5.36, boost, 0.1);
}

TEST(EpsCheckout, CanSenseEps5VRail1) {
    double rail_5v =
        i2c_measurable_manager->ReadI2cMeasurable<double>(kEps5VRail1, 0);
    DOUBLES_EQUAL(5.25, rail_5v, 0.25);
}

TEST(EpsCheckout, CanSenseEps5VRail2) {
    double rail_5v =
        i2c_measurable_manager->ReadI2cMeasurable<double>(kEps5VRail2, 0);
    DOUBLES_EQUAL(5.25, rail_5v, 0.25);
}

TEST(EpsCheckout, CanSensePowerTemp1) {
    double temp =
        i2c_measurable_manager->ReadI2cMeasurable<double>(kPowerTemp1, 0);
    DOUBLES_EQUAL(50, temp, 50);
}

TEST(EpsCheckout, CanSensePowerTemp2) {
    double temp =
        i2c_measurable_manager->ReadI2cMeasurable<double>(kPowerTemp2, 0);
    DOUBLES_EQUAL(50, temp, 50);
}

TEST(EpsCheckout, CanSensePowerBmsDieTemp1) {
    double temp =
        i2c_measurable_manager->ReadI2cMeasurable<double>(kPowerBmsDieTemp1, 0);
    DOUBLES_EQUAL(50, temp, 50);
}

TEST(EpsCheckout, CanSensePowerBmsDieTemp2) {
    double temp =
        i2c_measurable_manager->ReadI2cMeasurable<double>(kPowerBmsDieTemp2, 0);
    DOUBLES_EQUAL(50, temp, 50);
}

TEST(EpsCheckout, CanSensePowerBoostInCurrent1) {
    double current = i2c_measurable_manager->ReadI2cMeasurable<double>(
        kPowerBoostInCurrent1, 0);
    DOUBLES_EQUAL(2.5, current, 2.5);
}

TEST(EpsCheckout, CanSensePowerLoadCurrent1) {
    double current = i2c_measurable_manager->ReadI2cMeasurable<double>(
        kPowerLoadCurrent1, 0);
    DOUBLES_EQUAL(1.25, current, 1.25);
}

TEST(EpsCheckout, CanSensePowerLoadCurrent2) {
    double current = i2c_measurable_manager->ReadI2cMeasurable<double>(
        kPowerLoadCurrent2, 0);
    DOUBLES_EQUAL(1.25, current, 1.25);
}

TEST(EpsCheckout, CanSensePowerBoostInCurrent2) {
    double current = i2c_measurable_manager->ReadI2cMeasurable<double>(
        kPowerBoostInCurrent2, 0);
    DOUBLES_EQUAL(2.5, current, 2.5);
}
