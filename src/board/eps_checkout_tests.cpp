#include <CppUTest/TestHarness.h>
#include <src/config/unit_tests.h>
#include <src/sensors/i2c_measurable_manager.h>
#include <src/sensors/i2c_sensors/measurables/imu_temperature_measurable.h>

TEST_GROUP(EpsCheckout) {
    void setup() {
        if (!kEpsBoardAvailable) {
            TEST_EXIT
        }
    };
};

static I2cMeasurableManager* i2c_measurable_manager =
    I2cMeasurableManager::GetInstance();

TEST(EpsCheckout, CanSenseEpsBatVoltage1) {
    double vbat =
        i2c_measurable_manager->ReadI2cMeasurable<double>(kEpsAdcBatV1, 0);
    DOUBLES_EQUAL(2.9, vbat, 0.9);
}

TEST(EpsCheckout, CanSenseEpsBatVoltage2) {
    double vbat =
        i2c_measurable_manager->ReadI2cMeasurable<double>(kEpsAdcBatV2, 0);
    DOUBLES_EQUAL(2.9, vbat, 0.9);
}

TEST(EpsCheckout, CanSenseEpsBoostOutVoltage1) {
    double boost = i2c_measurable_manager->ReadI2cMeasurable<double>(
        kEpsBoostOutV1, 0);
    DOUBLES_EQUAL(5.36, boost, 0.1);
}

TEST(EpsCheckout, CanSenseEpsBoostOutVoltage2) {
    double boost = i2c_measurable_manager->ReadI2cMeasurable<double>(
        kEpsBoostOutV2, 0);
    DOUBLES_EQUAL(5.36, boost, 0.1);
}

TEST(EpsCheckout, CanSenseEps5VRail1) {
    double rail_5v =
        i2c_measurable_manager->ReadI2cMeasurable<double>(kEpsRail1, 0);
    DOUBLES_EQUAL(5.25, rail_5v, 0.25);
}

TEST(EpsCheckout, CanSenseEps5VRail2) {
    double rail_5v =
        i2c_measurable_manager->ReadI2cMeasurable<double>(kEpsRail2, 0);
    DOUBLES_EQUAL(5.25, rail_5v, 0.25);
}

TEST(EpsCheckout, CanSensePowerTemp1) {
    double temp =
        i2c_measurable_manager->ReadI2cMeasurable<double>(kEpsT1, 0);
    DOUBLES_EQUAL(50, temp, 50);
}

TEST(EpsCheckout, CanSensePowerTemp2) {
    double temp =
        i2c_measurable_manager->ReadI2cMeasurable<double>(kEpsT2, 0);
    DOUBLES_EQUAL(50, temp, 50);
}

TEST(EpsCheckout, CanSensePowerBmsDieTemp1) {
    double temp =
        i2c_measurable_manager->ReadI2cMeasurable<double>(kEpsBmsDieT1, 0);
    DOUBLES_EQUAL(50, temp, 50);
}

TEST(EpsCheckout, CanSensePowerBmsDieTemp2) {
    double temp =
        i2c_measurable_manager->ReadI2cMeasurable<double>(kEpsBmsDieT2, 0);
    DOUBLES_EQUAL(50, temp, 50);
}

TEST(EpsCheckout, CanSensePowerBoostInCurrent1) {
    double current = i2c_measurable_manager->ReadI2cMeasurable<double>(
        kEpsBoostInI1, 0);
    DOUBLES_EQUAL(2.5, current, 2.5);
}

TEST(EpsCheckout, CanSensePowerLoadCurrent1) {
    double current = i2c_measurable_manager->ReadI2cMeasurable<double>(
        kEpsLoadI1, 0);
    DOUBLES_EQUAL(1.25, current, 1.25);
}

TEST(EpsCheckout, CanSensePowerLoadCurrent2) {
    double current = i2c_measurable_manager->ReadI2cMeasurable<double>(
        kEpsBoostInI2, 0);
    DOUBLES_EQUAL(1.25, current, 1.25);
}

TEST(EpsCheckout, CanSensePowerBoostInCurrent2) {
    double current = i2c_measurable_manager->ReadI2cMeasurable<double>(
        kEpsLoadI2, 0);
    DOUBLES_EQUAL(2.5, current, 2.5);
}
