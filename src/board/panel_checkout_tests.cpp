#include <CppUTest/TestHarness.h>
#include <src/config/unit_tests.h>
#include <src/sensors/i2c_measurable_manager.h>
#include <src/sensors/i2c_sensors/measurables/imu_temperature_measurable.h>

TEST_GROUP(PanelCheckout){};

static I2cMeasurableManager* i2c_measurable_manager =
    I2cMeasurableManager::GetInstance();

TEST(PanelCheckout, XPosTemp1) {
    if (!kXPosAvailable) {
        TEST_EXIT
    }
    double temp = i2c_measurable_manager->ReadI2cMeasurable<double>(kXPosT1, 0);
    DOUBLES_EQUAL(20, temp, 45);
}

TEST(PanelCheckout, XPosTemp2) {
    if (!kXPosAvailable) {
        TEST_EXIT
    }
    double temp = i2c_measurable_manager->ReadI2cMeasurable<double>(kXPosT2, 0);
    DOUBLES_EQUAL(20, temp, 45);
}

TEST(PanelCheckout, YPosTemp1) {
    if (!kYPosAvailable) {
        TEST_EXIT
    }
    double temp = i2c_measurable_manager->ReadI2cMeasurable<double>(kYPosT1, 0);
    DOUBLES_EQUAL(20, temp, 45);
}

TEST(PanelCheckout, YPosTemp2) {
    if (!kYPosAvailable) {
        TEST_EXIT
    }
    double temp = i2c_measurable_manager->ReadI2cMeasurable<double>(kYPosT2, 0);
    DOUBLES_EQUAL(20, temp, 45);
}

TEST(PanelCheckout, XNegTemp1) {
    if (!kXNegAvailable) {
        TEST_EXIT
    }
    double temp = i2c_measurable_manager->ReadI2cMeasurable<double>(kXNegT1, 0);
    DOUBLES_EQUAL(20, temp, 45);
}

TEST(PanelCheckout, XNegTemp2) {
    if (!kXNegAvailable) {
        TEST_EXIT
    }
    double temp = i2c_measurable_manager->ReadI2cMeasurable<double>(kXNegT2, 0);
    DOUBLES_EQUAL(20, temp, 45);
}

TEST(PanelCheckout, YNegTemp1) {
    if (!kYNegAvailable) {
        TEST_EXIT
    }
    double temp = i2c_measurable_manager->ReadI2cMeasurable<double>(kYNegT1, 0);
    DOUBLES_EQUAL(20, temp, 45);
}

TEST(PanelCheckout, YNegTemp2) {
    if (!kYNegAvailable) {
        TEST_EXIT
    }
    double temp = i2c_measurable_manager->ReadI2cMeasurable<double>(kYNegT2, 0);
    DOUBLES_EQUAL(20, temp, 45);
}

TEST(PanelCheckout, ZNegTemp1) {
    if (!kZNegAvailable) {
        TEST_EXIT
    }
    double temp = i2c_measurable_manager->ReadI2cMeasurable<double>(kZNegT1, 0);
    DOUBLES_EQUAL(20, temp, 45);
}

TEST(PanelCheckout, ZNegTemp2) {
    if (!kZNegAvailable) {
        TEST_EXIT
    }
    double temp = i2c_measurable_manager->ReadI2cMeasurable<double>(kZNegT2, 0);
    DOUBLES_EQUAL(20, temp, 45);
}

TEST(PanelCheckout, ZPosTemp1) {
    if (!kZPosAvailable) {
        TEST_EXIT
    }
    double temp = i2c_measurable_manager->ReadI2cMeasurable<double>(kZPosT, 0);
    DOUBLES_EQUAL(20, temp, 45);
}

TEST(PanelCheckout, XPosVoltage) {
    if (!kXPosAvailable) {
        TEST_EXIT
    }
    double voltage =
        i2c_measurable_manager->ReadI2cMeasurable<double>(kXPosV, 0);
    DOUBLES_EQUAL(2.5, voltage, 3);
}

TEST(PanelCheckout, XPosSolarVoltage) {
    if (!kXPosAvailable) {
        TEST_EXIT
    }
    double voltage =
        i2c_measurable_manager->ReadI2cMeasurable<double>(kXPosSolarV, 0);
    DOUBLES_EQUAL(2.5, voltage, 3);
}

TEST(PanelCheckout, YPosVoltage) {
    if (!kYPosAvailable) {
        TEST_EXIT
    }
    double voltage =
        i2c_measurable_manager->ReadI2cMeasurable<double>(kYPosV, 0);
    DOUBLES_EQUAL(2.5, voltage, 3);
}

TEST(PanelCheckout, YPosSolarVoltage) {
    if (!kYPosAvailable) {
        TEST_EXIT
    }
    double voltage =
        i2c_measurable_manager->ReadI2cMeasurable<double>(kYPosSolarV, 0);
    DOUBLES_EQUAL(2.5, voltage, 3);
}

TEST(PanelCheckout, XNegVoltage) {
    if (!kXNegAvailable) {
        TEST_EXIT
    }
    double voltage =
        i2c_measurable_manager->ReadI2cMeasurable<double>(kXNegV, 0);
    DOUBLES_EQUAL(2.5, voltage, 3);
}

TEST(PanelCheckout, XNegSolarVoltage) {
    if (!kXNegAvailable) {
        TEST_EXIT
    }
    double voltage =
        i2c_measurable_manager->ReadI2cMeasurable<double>(kXNegSolarV, 0);
    DOUBLES_EQUAL(2.5, voltage, 3);
}

TEST(PanelCheckout, YNegVoltage) {
    if (!kYNegAvailable) {
        TEST_EXIT
    }
    double voltage =
        i2c_measurable_manager->ReadI2cMeasurable<double>(kYNegV, 0);
    DOUBLES_EQUAL(2.5, voltage, 3);
}

TEST(PanelCheckout, YNegSolarVoltage) {
    if (!kYNegAvailable) {
        TEST_EXIT
    }
    double voltage =
        i2c_measurable_manager->ReadI2cMeasurable<double>(kYNegSolarV, 0);
    DOUBLES_EQUAL(2.5, voltage, 3);
}

TEST(PanelCheckout, ZNegVoltage) {
    if (!kZNegAvailable) {
        TEST_EXIT
    }
    double voltage =
        i2c_measurable_manager->ReadI2cMeasurable<double>(kZNegV, 0);
    DOUBLES_EQUAL(2.5, voltage, 3);
}

TEST(PanelCheckout, ZNegSolarVoltage) {
    if (!kZNegAvailable) {
        TEST_EXIT
    }
    double voltage =
        i2c_measurable_manager->ReadI2cMeasurable<double>(kZNegSolarV, 0);
    DOUBLES_EQUAL(2.5, voltage, 3);
}

TEST(PanelCheckout, ZPosVoltage) {
    if (!kZPosAvailable) {
        TEST_EXIT
    }
    double voltage =
        i2c_measurable_manager->ReadI2cMeasurable<double>(kEpsTopPanelV, 0);
    DOUBLES_EQUAL(2.5, voltage, 3);
}

TEST(PanelCheckout, ZPosSolarVoltage) {
    if (!kZPosAvailable) {
        TEST_EXIT
    }
    double voltage =
        i2c_measurable_manager->ReadI2cMeasurable<double>(kEpsTopSolarV, 0);
    DOUBLES_EQUAL(2.5, voltage, 3);
}

TEST(PanelCheckout, ZPosSolarCurrent) {
    if (!kZPosAvailable) {
        TEST_EXIT
    }
    double current =
        i2c_measurable_manager->ReadI2cMeasurable<double>(kEpsTopSolarI, 0);
    DOUBLES_EQUAL(0.25, current, 0.25);
}

TEST(PanelCheckout, ZPosCurrent) {
    if (!kZPosAvailable) {
        TEST_EXIT
    }
    double current =
        i2c_measurable_manager->ReadI2cMeasurable<double>(kEpsTopPanelI, 0);
    DOUBLES_EQUAL(0.25, current, 0.25);
}

TEST(PanelCheckout, XPosSolarCurrent) {
    if (!kXPosAvailable) {
        TEST_EXIT
    }
    double current =
        i2c_measurable_manager->ReadI2cMeasurable<double>(kXPosSolarI, 0);
    DOUBLES_EQUAL(0.25, current, 0.25);
}

TEST(PanelCheckout, XPosCurrent) {
    if (!kXPosAvailable) {
        TEST_EXIT
    }
    double current =
        i2c_measurable_manager->ReadI2cMeasurable<double>(kXPosI, 0);
    DOUBLES_EQUAL(0.25, current, 0.25);
}

TEST(PanelCheckout, YPosSolarCurrent) {
    if (!kYPosAvailable) {
        TEST_EXIT
    }
    double current =
        i2c_measurable_manager->ReadI2cMeasurable<double>(kYPosSolarI, 0);
    DOUBLES_EQUAL(0.25, current, 0.25);
}

TEST(PanelCheckout, YPosCurrent) {
    if (!kYPosAvailable) {
        TEST_EXIT
    }
    double current =
        i2c_measurable_manager->ReadI2cMeasurable<double>(kYPosI, 0);
    DOUBLES_EQUAL(0.25, current, 0.25);
}

TEST(PanelCheckout, XNegSolarCurrent) {
    if (!kXNegAvailable) {
        TEST_EXIT
    }
    double current =
        i2c_measurable_manager->ReadI2cMeasurable<double>(kXNegSolarI, 0);
    DOUBLES_EQUAL(0.25, current, 0.25);
}

TEST(PanelCheckout, XNegCurrent) {
    if (!kXNegAvailable) {
        TEST_EXIT
    }
    double current =
        i2c_measurable_manager->ReadI2cMeasurable<double>(kXNegI, 0);
    DOUBLES_EQUAL(0.25, current, 0.25);
}

TEST(PanelCheckout, YNegSolarCurrent) {
    if (!kYNegAvailable) {
        TEST_EXIT
    }
    double current =
        i2c_measurable_manager->ReadI2cMeasurable<double>(kYNegSolarI, 0);
    DOUBLES_EQUAL(0.25, current, 0.25);
}

TEST(PanelCheckout, YNegCurrent) {
    if (!kYNegAvailable) {
        TEST_EXIT
    }
    double current =
        i2c_measurable_manager->ReadI2cMeasurable<double>(kYNegI, 0);
    DOUBLES_EQUAL(0.25, current, 0.25);
}

TEST(PanelCheckout, ZNegSolarCurrent) {
    if (!kZNegAvailable) {
        TEST_EXIT
    }
    double current =
        i2c_measurable_manager->ReadI2cMeasurable<double>(kZNegSolarI, 0);
    DOUBLES_EQUAL(0.25, current, 0.25);
}

TEST(PanelCheckout, ZNegCurrent) {
    if (!kZNegAvailable) {
        TEST_EXIT
    }
    double current =
        i2c_measurable_manager->ReadI2cMeasurable<double>(kZNegI, 0);
    DOUBLES_EQUAL(0.25, current, 0.25);
}
