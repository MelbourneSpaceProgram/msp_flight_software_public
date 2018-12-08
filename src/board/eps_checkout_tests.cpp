#include <CppUTest/TestHarness.h>
#include <src/config/unit_tests.h>
#include <src/messages/CurrentReading.pb.h>
#include <src/messages/TemperatureReading.pb.h>
#include <src/messages/VoltageReading.pb.h>
#include <src/sensors/measurable_manager.h>
#include <src/sensors/i2c_sensors/measurables/imu_temperature_measurable.h>

TEST_GROUP(EpsCheckout) {
    void setup() {
        if (!SystemConfiguration::GetInstance()->IsEpsBoardAvailable()) {
            TEST_EXIT
        }
    };
};

static MeasurableManager* measurable_manager =
    MeasurableManager::GetInstance();

TEST(EpsCheckout, CanSenseEpsBatVoltage1) {
    double vbat = measurable_manager
                      ->ReadNanopbMeasurable<VoltageReading>(kEpsAdcBatV1, 0)
                      .voltage;
    DOUBLES_EQUAL(2.9, vbat, 0.9);
}

TEST(EpsCheckout, CanSenseEpsBatVoltage2) {
    double vbat = measurable_manager
                      ->ReadNanopbMeasurable<VoltageReading>(kEpsAdcBatV2, 0)
                      .voltage;
    DOUBLES_EQUAL(2.9, vbat, 0.9);
}

TEST(EpsCheckout, CanSenseEpsBoostOutVoltage1) {
    double boost = measurable_manager
                       ->ReadNanopbMeasurable<VoltageReading>(kEpsBoostOutV1, 0)
                       .voltage;
    DOUBLES_EQUAL(5.36, boost, 0.1);
}

TEST(EpsCheckout, CanSenseEpsBoostOutVoltage2) {
    double boost = measurable_manager
                       ->ReadNanopbMeasurable<VoltageReading>(kEpsBoostOutV2, 0)
                       .voltage;
    DOUBLES_EQUAL(5.36, boost, 0.1);
}

TEST(EpsCheckout, CanSenseEps5VRail1) {
    double rail_5v =
        measurable_manager->ReadNanopbMeasurable<VoltageReading>(kEpsRail1, 0)
            .voltage;
    DOUBLES_EQUAL(5.25, rail_5v, 0.25);
}

TEST(EpsCheckout, CanSenseEps5VRail2) {
    double rail_5v =
        measurable_manager->ReadNanopbMeasurable<VoltageReading>(kEpsRail2, 0)
            .voltage;
    DOUBLES_EQUAL(5.25, rail_5v, 0.25);
}

TEST(EpsCheckout, CanSensePowerTemp1) {
    double temp =
        measurable_manager->ReadNanopbMeasurable<TemperatureReading>(kEpsT1, 0)
            .temp;
    DOUBLES_EQUAL(20, temp, 45);
}

TEST(EpsCheckout, CanSensePowerTemp2) {
    double temp =
        measurable_manager->ReadNanopbMeasurable<TemperatureReading>(kEpsT2, 0)
            .temp;
    DOUBLES_EQUAL(20, temp, 45);
}

TEST(EpsCheckout, CanSensePowerBmsDieTemp1) {
    double temp = measurable_manager
                      ->ReadNanopbMeasurable<TemperatureReading>(kEpsBmsDieT1, 0)
                      .temp;
    DOUBLES_EQUAL(20, temp, 45);
}

TEST(EpsCheckout, CanSensePowerBmsDieTemp2) {
    double temp = measurable_manager
                      ->ReadNanopbMeasurable<TemperatureReading>(kEpsBmsDieT2, 0)
                      .temp;
    DOUBLES_EQUAL(20, temp, 45);
}

TEST(EpsCheckout, CanSensePowerBoostInCurrent1) {
    double current = measurable_manager
                         ->ReadNanopbMeasurable<CurrentReading>(kEpsBoostInI1, 0)
                         .current;
    DOUBLES_EQUAL(2.5, current, 2.5);
}

TEST(EpsCheckout, CanSensePowerLoadCurrent1) {
    double current =
        measurable_manager->ReadNanopbMeasurable<CurrentReading>(kEpsLoadI1, 0)
            .current;
    DOUBLES_EQUAL(1.25, current, 1.25);
}

TEST(EpsCheckout, CanSensePowerLoadCurrent2) {
    double current = measurable_manager
                         ->ReadNanopbMeasurable<CurrentReading>(kEpsBoostInI2, 0)
                         .current;
    DOUBLES_EQUAL(1.25, current, 1.25);
}

TEST(EpsCheckout, CanSensePowerBoostInCurrent2) {
    double current =
        measurable_manager->ReadNanopbMeasurable<CurrentReading>(kEpsLoadI2, 0)
            .current;
    DOUBLES_EQUAL(2.5, current, 2.5);
}
