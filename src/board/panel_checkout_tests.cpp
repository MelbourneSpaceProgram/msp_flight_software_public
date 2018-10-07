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
    if (!kXPosAvailable) {
        TEST_EXIT
    }
    double temp = measurable_manager
                      ->ReadNanopbMeasurable<TemperatureReading>(kXPosT1, 0)
                      .temp;
    DOUBLES_EQUAL(20, temp, 45);
}

TEST(PanelCheckout, XPosTemp2) {
    if (!kXPosAvailable) {
        TEST_EXIT
    }
    double temp = measurable_manager
                      ->ReadNanopbMeasurable<TemperatureReading>(kXPosT2, 0)
                      .temp;
    DOUBLES_EQUAL(20, temp, 45);
}

TEST(PanelCheckout, YPosTemp1) {
    if (!kYPosAvailable) {
        TEST_EXIT
    }
    double temp = measurable_manager
                      ->ReadNanopbMeasurable<TemperatureReading>(kYPosT1, 0)
                      .temp;
    DOUBLES_EQUAL(20, temp, 45);
}

TEST(PanelCheckout, YPosTemp2) {
    if (!kYPosAvailable) {
        TEST_EXIT
    }
    double temp = measurable_manager
                      ->ReadNanopbMeasurable<TemperatureReading>(kYPosT2, 0)
                      .temp;
    DOUBLES_EQUAL(20, temp, 45);
}

TEST(PanelCheckout, XNegTemp1) {
    if (!kXNegAvailable) {
        TEST_EXIT
    }
    double temp = measurable_manager
                      ->ReadNanopbMeasurable<TemperatureReading>(kXNegT1, 0)
                      .temp;
    DOUBLES_EQUAL(20, temp, 45);
}

TEST(PanelCheckout, XNegTemp2) {
    if (!kXNegAvailable) {
        TEST_EXIT
    }
    double temp = measurable_manager
                      ->ReadNanopbMeasurable<TemperatureReading>(kXNegT2, 0)
                      .temp;
    DOUBLES_EQUAL(20, temp, 45);
}

TEST(PanelCheckout, YNegTemp1) {
    if (!kYNegAvailable) {
        TEST_EXIT
    }
    double temp = measurable_manager
                      ->ReadNanopbMeasurable<TemperatureReading>(kYNegT1, 0)
                      .temp;
    DOUBLES_EQUAL(20, temp, 45);
}

TEST(PanelCheckout, YNegTemp2) {
    if (!kYNegAvailable) {
        TEST_EXIT
    }
    double temp = measurable_manager
                      ->ReadNanopbMeasurable<TemperatureReading>(kYNegT2, 0)
                      .temp;
    DOUBLES_EQUAL(20, temp, 45);
}

TEST(PanelCheckout, ZNegTemp1) {
    if (!kZNegAvailable) {
        TEST_EXIT
    }
    double temp = measurable_manager
                      ->ReadNanopbMeasurable<TemperatureReading>(kZNegT1, 0)
                      .temp;
    DOUBLES_EQUAL(20, temp, 45);
}

TEST(PanelCheckout, ZNegTemp2) {
    if (!kZNegAvailable) {
        TEST_EXIT
    }
    double temp = measurable_manager
                      ->ReadNanopbMeasurable<TemperatureReading>(kZNegT2, 0)
                      .temp;
    DOUBLES_EQUAL(20, temp, 45);
}

TEST(PanelCheckout, ZPosTemp1) {
    if (!kZPosAvailable) {
        TEST_EXIT
    }
    double temp =
        measurable_manager->ReadNanopbMeasurable<TemperatureReading>(kZPosT, 0)
            .temp;
    DOUBLES_EQUAL(20, temp, 45);
}

TEST(PanelCheckout, XPosVoltage) {
    if (!kXPosAvailable) {
        TEST_EXIT
    }
    double voltage =
        measurable_manager->ReadNanopbMeasurable<VoltageReading>(kXPosV, 0)
            .voltage;
    DOUBLES_EQUAL(2.5, voltage, 3);
}

TEST(PanelCheckout, XPosSolarVoltage) {
    if (!kXPosAvailable) {
        TEST_EXIT
    }
    double voltage = measurable_manager
                         ->ReadNanopbMeasurable<VoltageReading>(kXPosSolarV, 0)
                         .voltage;
    DOUBLES_EQUAL(2.5, voltage, 3);
}

TEST(PanelCheckout, YPosVoltage) {
    if (!kYPosAvailable) {
        TEST_EXIT
    }
    double voltage =
        measurable_manager->ReadNanopbMeasurable<VoltageReading>(kYPosV, 0)
            .voltage;
    DOUBLES_EQUAL(2.5, voltage, 3);
}

TEST(PanelCheckout, YPosSolarVoltage) {
    if (!kYPosAvailable) {
        TEST_EXIT
    }
    double voltage = measurable_manager
                         ->ReadNanopbMeasurable<VoltageReading>(kYPosSolarV, 0)
                         .voltage;
    DOUBLES_EQUAL(2.5, voltage, 3);
}

TEST(PanelCheckout, XNegVoltage) {
    if (!kXNegAvailable) {
        TEST_EXIT
    }
    double voltage =
        measurable_manager->ReadNanopbMeasurable<VoltageReading>(kXNegV, 0)
            .voltage;
    DOUBLES_EQUAL(2.5, voltage, 3);
}

TEST(PanelCheckout, XNegSolarVoltage) {
    if (!kXNegAvailable) {
        TEST_EXIT
    }
    double voltage = measurable_manager
                         ->ReadNanopbMeasurable<VoltageReading>(kXNegSolarV, 0)
                         .voltage;
    DOUBLES_EQUAL(2.5, voltage, 3);
}

TEST(PanelCheckout, YNegVoltage) {
    if (!kYNegAvailable) {
        TEST_EXIT
    }
    double voltage =
        measurable_manager->ReadNanopbMeasurable<VoltageReading>(kYNegV, 0)
            .voltage;
    DOUBLES_EQUAL(2.5, voltage, 3);
}

TEST(PanelCheckout, YNegSolarVoltage) {
    if (!kYNegAvailable) {
        TEST_EXIT
    }
    double voltage = measurable_manager
                         ->ReadNanopbMeasurable<VoltageReading>(kYNegSolarV, 0)
                         .voltage;
    DOUBLES_EQUAL(2.5, voltage, 3);
}

TEST(PanelCheckout, ZNegVoltage) {
    if (!kZNegAvailable) {
        TEST_EXIT
    }
    double voltage =
        measurable_manager->ReadNanopbMeasurable<VoltageReading>(kZNegV, 0)
            .voltage;
    DOUBLES_EQUAL(2.5, voltage, 3);
}

TEST(PanelCheckout, ZNegSolarVoltage) {
    if (!kZNegAvailable) {
        TEST_EXIT
    }
    double voltage = measurable_manager
                         ->ReadNanopbMeasurable<VoltageReading>(kZNegSolarV, 0)
                         .voltage;
    DOUBLES_EQUAL(2.5, voltage, 3);
}

TEST(PanelCheckout, ZPosVoltage) {
    if (!kZPosAvailable) {
        TEST_EXIT
    }
    double voltage = measurable_manager
                         ->ReadNanopbMeasurable<VoltageReading>(kEpsTopPanelV, 0)
                         .voltage;
    DOUBLES_EQUAL(2.5, voltage, 3);
}

TEST(PanelCheckout, ZPosSolarVoltage) {
    if (!kZPosAvailable) {
        TEST_EXIT
    }
    double voltage = measurable_manager
                         ->ReadNanopbMeasurable<VoltageReading>(kEpsTopSolarV, 0)
                         .voltage;
    DOUBLES_EQUAL(2.5, voltage, 3);
}

TEST(PanelCheckout, ZPosSolarCurrent) {
    if (!kZPosAvailable) {
        TEST_EXIT
    }
    double current = measurable_manager
                         ->ReadNanopbMeasurable<CurrentReading>(kEpsTopSolarI, 0)
                         .current;
    DOUBLES_EQUAL(0.25, current, 0.25);
}

TEST(PanelCheckout, ZPosCurrent) {
    if (!kZPosAvailable) {
        TEST_EXIT
    }
    double current = measurable_manager
                         ->ReadNanopbMeasurable<CurrentReading>(kEpsTopPanelI, 0)
                         .current;
    DOUBLES_EQUAL(0.25, current, 0.25);
}

TEST(PanelCheckout, XPosSolarCurrent) {
    if (!kXPosAvailable) {
        TEST_EXIT
    }
    double current = measurable_manager
                         ->ReadNanopbMeasurable<CurrentReading>(kXPosSolarI, 0)
                         .current;
    DOUBLES_EQUAL(0.25, current, 0.25);
}

TEST(PanelCheckout, XPosCurrent) {
    if (!kXPosAvailable) {
        TEST_EXIT
    }
    double current =
        measurable_manager->ReadNanopbMeasurable<CurrentReading>(kXPosI, 0)
            .current;
    DOUBLES_EQUAL(0.25, current, 0.25);
}

TEST(PanelCheckout, YPosSolarCurrent) {
    if (!kYPosAvailable) {
        TEST_EXIT
    }
    double current = measurable_manager
                         ->ReadNanopbMeasurable<CurrentReading>(kYPosSolarI, 0)
                         .current;
    DOUBLES_EQUAL(0.25, current, 0.25);
}

TEST(PanelCheckout, YPosCurrent) {
    if (!kYPosAvailable) {
        TEST_EXIT
    }
    double current =
        measurable_manager->ReadNanopbMeasurable<CurrentReading>(kYPosI, 0)
            .current;
    DOUBLES_EQUAL(0.25, current, 0.25);
}

TEST(PanelCheckout, XNegSolarCurrent) {
    if (!kXNegAvailable) {
        TEST_EXIT
    }
    double current = measurable_manager
                         ->ReadNanopbMeasurable<CurrentReading>(kXNegSolarI, 0)
                         .current;
    DOUBLES_EQUAL(0.25, current, 0.25);
}

TEST(PanelCheckout, XNegCurrent) {
    if (!kXNegAvailable) {
        TEST_EXIT
    }
    double current =
        measurable_manager->ReadNanopbMeasurable<CurrentReading>(kXNegI, 0)
            .current;
    DOUBLES_EQUAL(0.25, current, 0.25);
}

TEST(PanelCheckout, YNegSolarCurrent) {
    if (!kYNegAvailable) {
        TEST_EXIT
    }
    double current = measurable_manager
                         ->ReadNanopbMeasurable<CurrentReading>(kYNegSolarI, 0)
                         .current;
    DOUBLES_EQUAL(0.25, current, 0.25);
}

TEST(PanelCheckout, YNegCurrent) {
    if (!kYNegAvailable) {
        TEST_EXIT
    }
    double current =
        measurable_manager->ReadNanopbMeasurable<CurrentReading>(kYNegI, 0)
            .current;
    DOUBLES_EQUAL(0.25, current, 0.25);
}

TEST(PanelCheckout, ZNegSolarCurrent) {
    if (!kZNegAvailable) {
        TEST_EXIT
    }
    double current = measurable_manager
                         ->ReadNanopbMeasurable<CurrentReading>(kZNegSolarI, 0)
                         .current;
    DOUBLES_EQUAL(0.25, current, 0.25);
}

TEST(PanelCheckout, ZNegCurrent) {
    if (!kZNegAvailable) {
        TEST_EXIT
    }
    double current =
        measurable_manager->ReadNanopbMeasurable<CurrentReading>(kZNegI, 0)
            .current;
    DOUBLES_EQUAL(0.25, current, 0.25);
}
