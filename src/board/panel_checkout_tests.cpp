#include <CppUTest/TestHarness.h>
#include <src/config/unit_tests.h>
#include <src/messages/CurrentReading.pb.h>
#include <src/messages/TemperatureReading.pb.h>
#include <src/messages/VoltageReading.pb.h>
#include <src/sensors/measurable_manager.h>
#include <src/sensors/i2c_sensors/measurables/imu_temperature_measurable.h>

TEST_GROUP(PanelCheckout){};

static MeasurableManager* measurable_manager =
    MeasurableManager::GetInstance();

TEST(PanelCheckout, XPosTemp1) {
    if (!SystemConfiguration::GetInstance()->IsXPosAvailable()) {
        TEST_EXIT
    }
    double temp = measurable_manager
                      ->ReadNanopbMeasurable<TemperatureReading>(kXPosT1, 0)
                      .temp;
    DOUBLES_EQUAL(20, temp, 45);
}

TEST(PanelCheckout, XPosTemp2) {
    if (!SystemConfiguration::GetInstance()->IsXPosAvailable()) {
        TEST_EXIT
    }
    double temp = measurable_manager
                      ->ReadNanopbMeasurable<TemperatureReading>(kXPosT2, 0)
                      .temp;
    DOUBLES_EQUAL(20, temp, 45);
}

TEST(PanelCheckout, YPosTemp1) {
    if (!SystemConfiguration::GetInstance()->IsYPosAvailable()) {
        TEST_EXIT
    }
    double temp = measurable_manager
                      ->ReadNanopbMeasurable<TemperatureReading>(kYPosT1, 0)
                      .temp;
    DOUBLES_EQUAL(20, temp, 45);
}

TEST(PanelCheckout, YPosTemp2) {
    if (!SystemConfiguration::GetInstance()->IsYPosAvailable()) {
        TEST_EXIT
    }
    double temp = measurable_manager
                      ->ReadNanopbMeasurable<TemperatureReading>(kYPosT2, 0)
                      .temp;
    DOUBLES_EQUAL(20, temp, 45);
}

TEST(PanelCheckout, XNegTemp1) {
    if (!SystemConfiguration::GetInstance()->IsXNegAvailable()) {
        TEST_EXIT
    }
    double temp = measurable_manager
                      ->ReadNanopbMeasurable<TemperatureReading>(kXNegT1, 0)
                      .temp;
    DOUBLES_EQUAL(20, temp, 45);
}

TEST(PanelCheckout, XNegTemp2) {
    if (!SystemConfiguration::GetInstance()->IsXNegAvailable()) {
        TEST_EXIT
    }
    double temp = measurable_manager
                      ->ReadNanopbMeasurable<TemperatureReading>(kXNegT2, 0)
                      .temp;
    DOUBLES_EQUAL(20, temp, 45);
}

TEST(PanelCheckout, YNegTemp1) {
    if (!SystemConfiguration::GetInstance()->IsYNegAvailable()) {
        TEST_EXIT
    }
    double temp = measurable_manager
                      ->ReadNanopbMeasurable<TemperatureReading>(kYNegT1, 0)
                      .temp;
    DOUBLES_EQUAL(20, temp, 45);
}

TEST(PanelCheckout, YNegTemp2) {
    if (!SystemConfiguration::GetInstance()->IsYNegAvailable()) {
        TEST_EXIT
    }
    double temp = measurable_manager
                      ->ReadNanopbMeasurable<TemperatureReading>(kYNegT2, 0)
                      .temp;
    DOUBLES_EQUAL(20, temp, 45);
}

TEST(PanelCheckout, ZNegTemp1) {
    if (!SystemConfiguration::GetInstance()->IsZNegAvailable()) {
        TEST_EXIT
    }
    double temp = measurable_manager
                      ->ReadNanopbMeasurable<TemperatureReading>(kZNegT1, 0)
                      .temp;
    DOUBLES_EQUAL(20, temp, 45);
}

TEST(PanelCheckout, ZNegTemp2) {
    if (!SystemConfiguration::GetInstance()->IsZNegAvailable()) {
        TEST_EXIT
    }
    double temp = measurable_manager
                      ->ReadNanopbMeasurable<TemperatureReading>(kZNegT2, 0)
                      .temp;
    DOUBLES_EQUAL(20, temp, 45);
}

TEST(PanelCheckout, ZPosTemp1) {
    if (!SystemConfiguration::GetInstance()->IsZPosAvailable()) {
        TEST_EXIT
    }
    double temp =
        measurable_manager->ReadNanopbMeasurable<TemperatureReading>(kZPosT, 0)
            .temp;
    DOUBLES_EQUAL(20, temp, 45);
}

TEST(PanelCheckout, XPosVoltage) {
    if (!SystemConfiguration::GetInstance()->IsXPosAvailable()) {
        TEST_EXIT
    }
    double voltage =
        measurable_manager->ReadNanopbMeasurable<VoltageReading>(kXPosV, 0)
            .voltage;
    DOUBLES_EQUAL(2.5, voltage, 3);
}

TEST(PanelCheckout, XPosSolarVoltage) {
    if (!SystemConfiguration::GetInstance()->IsXPosAvailable()) {
        TEST_EXIT
    }
    double voltage = measurable_manager
                         ->ReadNanopbMeasurable<VoltageReading>(kXPosSolarV, 0)
                         .voltage;
    DOUBLES_EQUAL(2.5, voltage, 3);
}

TEST(PanelCheckout, YPosVoltage) {
    if (!SystemConfiguration::GetInstance()->IsYPosAvailable()) {
        TEST_EXIT
    }
    double voltage =
        measurable_manager->ReadNanopbMeasurable<VoltageReading>(kYPosV, 0)
            .voltage;
    DOUBLES_EQUAL(2.5, voltage, 3);
}

TEST(PanelCheckout, YPosSolarVoltage) {
    if (!SystemConfiguration::GetInstance()->IsYPosAvailable()) {
        TEST_EXIT
    }
    double voltage = measurable_manager
                         ->ReadNanopbMeasurable<VoltageReading>(kYPosSolarV, 0)
                         .voltage;
    DOUBLES_EQUAL(2.5, voltage, 3);
}

TEST(PanelCheckout, XNegVoltage) {
    if (!SystemConfiguration::GetInstance()->IsXNegAvailable()) {
        TEST_EXIT
    }
    double voltage =
        measurable_manager->ReadNanopbMeasurable<VoltageReading>(kXNegV, 0)
            .voltage;
    DOUBLES_EQUAL(2.5, voltage, 3);
}

TEST(PanelCheckout, XNegSolarVoltage) {
    if (!SystemConfiguration::GetInstance()->IsXNegAvailable()) {
        TEST_EXIT
    }
    double voltage = measurable_manager
                         ->ReadNanopbMeasurable<VoltageReading>(kXNegSolarV, 0)
                         .voltage;
    DOUBLES_EQUAL(2.5, voltage, 3);
}

TEST(PanelCheckout, YNegVoltage) {
    if (!SystemConfiguration::GetInstance()->IsYNegAvailable()) {
        TEST_EXIT
    }
    double voltage =
        measurable_manager->ReadNanopbMeasurable<VoltageReading>(kYNegV, 0)
            .voltage;
    DOUBLES_EQUAL(2.5, voltage, 3);
}

TEST(PanelCheckout, YNegSolarVoltage) {
    if (!SystemConfiguration::GetInstance()->IsYNegAvailable()) {
        TEST_EXIT
    }
    double voltage = measurable_manager
                         ->ReadNanopbMeasurable<VoltageReading>(kYNegSolarV, 0)
                         .voltage;
    DOUBLES_EQUAL(2.5, voltage, 3);
}

TEST(PanelCheckout, ZNegVoltage) {
    if (!SystemConfiguration::GetInstance()->IsZNegAvailable()) {
        TEST_EXIT
    }
    double voltage =
        measurable_manager->ReadNanopbMeasurable<VoltageReading>(kZNegV, 0)
            .voltage;
    DOUBLES_EQUAL(2.5, voltage, 3);
}

TEST(PanelCheckout, ZNegSolarVoltage) {
    if (!SystemConfiguration::GetInstance()->IsZNegAvailable()) {
        TEST_EXIT
    }
    double voltage = measurable_manager
                         ->ReadNanopbMeasurable<VoltageReading>(kZNegSolarV, 0)
                         .voltage;
    DOUBLES_EQUAL(2.5, voltage, 3);
}

TEST(PanelCheckout, ZPosVoltage) {
    if (!SystemConfiguration::GetInstance()->IsZPosAvailable()) {
        TEST_EXIT
    }
    double voltage = measurable_manager
                         ->ReadNanopbMeasurable<VoltageReading>(kEpsTopPanelV, 0)
                         .voltage;
    DOUBLES_EQUAL(2.5, voltage, 3);
}

TEST(PanelCheckout, ZPosSolarVoltage) {
    if (!SystemConfiguration::GetInstance()->IsZPosAvailable()) {
        TEST_EXIT
    }
    double voltage = measurable_manager
                         ->ReadNanopbMeasurable<VoltageReading>(kEpsTopSolarV, 0)
                         .voltage;
    DOUBLES_EQUAL(2.5, voltage, 3);
}

TEST(PanelCheckout, ZPosSolarCurrent) {
    if (!SystemConfiguration::GetInstance()->IsZPosAvailable()) {
        TEST_EXIT
    }
    double current = measurable_manager
                         ->ReadNanopbMeasurable<CurrentReading>(kEpsTopSolarI, 0)
                         .current;
    DOUBLES_EQUAL(0.25, current, 0.25);
}

TEST(PanelCheckout, ZPosCurrent) {
    if (!SystemConfiguration::GetInstance()->IsZPosAvailable()) {
        TEST_EXIT
    }
    double current = measurable_manager
                         ->ReadNanopbMeasurable<CurrentReading>(kEpsTopPanelI, 0)
                         .current;
    DOUBLES_EQUAL(0.25, current, 0.25);
}

TEST(PanelCheckout, XPosSolarCurrent) {
    if (!SystemConfiguration::GetInstance()->IsXPosAvailable()) {
        TEST_EXIT
    }
    double current = measurable_manager
                         ->ReadNanopbMeasurable<CurrentReading>(kXPosSolarI, 0)
                         .current;
    DOUBLES_EQUAL(0.25, current, 0.25);
}

TEST(PanelCheckout, XPosCurrent) {
    if (!SystemConfiguration::GetInstance()->IsXPosAvailable()) {
        TEST_EXIT
    }
    double current =
        measurable_manager->ReadNanopbMeasurable<CurrentReading>(kXPosI, 0)
            .current;
    DOUBLES_EQUAL(0.25, current, 0.25);
}

TEST(PanelCheckout, YPosSolarCurrent) {
    if (!SystemConfiguration::GetInstance()->IsYPosAvailable()) {
        TEST_EXIT
    }
    double current = measurable_manager
                         ->ReadNanopbMeasurable<CurrentReading>(kYPosSolarI, 0)
                         .current;
    DOUBLES_EQUAL(0.25, current, 0.25);
}

TEST(PanelCheckout, YPosCurrent) {
    if (!SystemConfiguration::GetInstance()->IsYPosAvailable()) {
        TEST_EXIT
    }
    double current =
        measurable_manager->ReadNanopbMeasurable<CurrentReading>(kYPosI, 0)
            .current;
    DOUBLES_EQUAL(0.25, current, 0.25);
}

TEST(PanelCheckout, XNegSolarCurrent) {
    if (!SystemConfiguration::GetInstance()->IsXNegAvailable()) {
        TEST_EXIT
    }
    double current = measurable_manager
                         ->ReadNanopbMeasurable<CurrentReading>(kXNegSolarI, 0)
                         .current;
    DOUBLES_EQUAL(0.25, current, 0.25);
}

TEST(PanelCheckout, XNegCurrent) {
    if (!SystemConfiguration::GetInstance()->IsXNegAvailable()) {
        TEST_EXIT
    }
    double current =
        measurable_manager->ReadNanopbMeasurable<CurrentReading>(kXNegI, 0)
            .current;
    DOUBLES_EQUAL(0.25, current, 0.25);
}

TEST(PanelCheckout, YNegSolarCurrent) {
    if (!SystemConfiguration::GetInstance()->IsYNegAvailable()) {
        TEST_EXIT
    }
    double current = measurable_manager
                         ->ReadNanopbMeasurable<CurrentReading>(kYNegSolarI, 0)
                         .current;
    DOUBLES_EQUAL(0.25, current, 0.25);
}

TEST(PanelCheckout, YNegCurrent) {
    if (!SystemConfiguration::GetInstance()->IsYNegAvailable()) {
        TEST_EXIT
    }
    double current =
        measurable_manager->ReadNanopbMeasurable<CurrentReading>(kYNegI, 0)
            .current;
    DOUBLES_EQUAL(0.25, current, 0.25);
}

TEST(PanelCheckout, ZNegSolarCurrent) {
    if (!SystemConfiguration::GetInstance()->IsZNegAvailable()) {
        TEST_EXIT
    }
    double current = measurable_manager
                         ->ReadNanopbMeasurable<CurrentReading>(kZNegSolarI, 0)
                         .current;
    DOUBLES_EQUAL(0.25, current, 0.25);
}

TEST(PanelCheckout, ZNegCurrent) {
    if (!SystemConfiguration::GetInstance()->IsZNegAvailable()) {
        TEST_EXIT
    }
    double current =
        measurable_manager->ReadNanopbMeasurable<CurrentReading>(kZNegI, 0)
            .current;
    DOUBLES_EQUAL(0.25, current, 0.25);
}
