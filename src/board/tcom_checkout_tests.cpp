#include <CppUTest/TestHarness.h>
#include <src/config/unit_tests.h>
#include <src/sensors/i2c_measurable_manager.h>

TEST_GROUP(TcomCheckout) {
    void setup() {
        if (!tcom_board_available) {
            TEST_EXIT
        }
    };
};

static I2cMeasurableManager* i2c_measurable_manager =
    I2cMeasurableManager::GetInstance();

TEST(TcomCheckout, CanSenseCommsTemp1) {
    double temp =
        i2c_measurable_manager->ReadI2cMeasurable<double>(kComT1, 0);
    DOUBLES_EQUAL(50, temp, 50);
}

TEST(TcomCheckout, CanSenseCommsTemp2) {
    double temp =
        i2c_measurable_manager->ReadI2cMeasurable<double>(kComT2, 0);
    DOUBLES_EQUAL(50, temp, 50);
}

TEST(TcomCheckout, CanSenseCommsRegulator1OutputVoltage) {
    double voltage = i2c_measurable_manager->ReadI2cMeasurable<double>(
        kComOutV1, 0);
    DOUBLES_EQUAL(8.2, voltage, 0.5);
}

TEST(TcomCheckout, CanSenseCommsRegulator1InputVoltage) {
    double voltage = i2c_measurable_manager->ReadI2cMeasurable<double>(
        kComInI1, 0);
    DOUBLES_EQUAL(5.2, voltage, 0.5);
}

TEST(TcomCheckout, CanSenseCommsRegulator2OutputVoltage) {
    double voltage = i2c_measurable_manager->ReadI2cMeasurable<double>(
        kComOutV2, 0);
    DOUBLES_EQUAL(8.2, voltage, 0.5);
}

TEST(TcomCheckout, CanSenseCommsRegulator2InputVoltage) {
    double voltage = i2c_measurable_manager->ReadI2cMeasurable<double>(
        kComInV2, 0);
    DOUBLES_EQUAL(5.2, voltage, 0.5);
}

TEST(TcomCheckout, CanSenseCommsRegulator1InputCurrent) {
    double current = i2c_measurable_manager->ReadI2cMeasurable<double>(
        kComInI1, 0);
    DOUBLES_EQUAL(1.5, current, 1.5);
}

TEST(TcomCheckout, CanSenseCommsRegulator1OutputCurrent) {
    double current = i2c_measurable_manager->ReadI2cMeasurable<double>(
        kComInI1, 0);
    DOUBLES_EQUAL(1.5, current, 1.5);
}

TEST(TcomCheckout, CanSenseCommsRegulator2InputCurrent) {
    double current = i2c_measurable_manager->ReadI2cMeasurable<double>(
        kComInI1, 0);
    DOUBLES_EQUAL(1.5, current, 1.5);
}

TEST(TcomCheckout, CanSenseCommsRegulator2OutputCurrent) {
    double current = i2c_measurable_manager->ReadI2cMeasurable<double>(
        kComOutI2, 0);
    DOUBLES_EQUAL(1.5, current, 1.5);
}
