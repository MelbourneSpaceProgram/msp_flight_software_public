#include <CppUTest/TestHarness.h>
#include <src/config/unit_tests.h>
#include <src/messages/CurrentReading.pb.h>
#include <src/messages/TemperatureReading.pb.h>
#include <src/messages/VoltageReading.pb.h>
#include <src/sensors/measurable_manager.h>

TEST_GROUP(TcomCheckout) {
    void setup() {
        if (!kTcomBoardAvailable) {
            TEST_EXIT
        }
    };
};

static MeasurableManager* measurable_manager =
    MeasurableManager::GetInstance();

TEST(TcomCheckout, CanSenseCommsTemp1) {
    double temp =
        measurable_manager->ReadNanopbMeasurable<TemperatureReading>(kComT1, 0)
            .temp;
    DOUBLES_EQUAL(20, temp, 45);
}

TEST(TcomCheckout, CanSenseCommsTemp2) {
    double temp =
        measurable_manager->ReadNanopbMeasurable<TemperatureReading>(kComT2, 0)
            .temp;
    DOUBLES_EQUAL(20, temp, 45);
}

TEST(TcomCheckout, CanSenseCommsRegulator1OutputVoltage) {
    double voltage =
        measurable_manager->ReadNanopbMeasurable<VoltageReading>(kComOutV1, 0)
            .voltage;
    DOUBLES_EQUAL(8.2, voltage, 0.5);
}

TEST(TcomCheckout, CanSenseCommsRegulator1InputVoltage) {
    double voltage =
        measurable_manager->ReadNanopbMeasurable<VoltageReading>(kComInV1, 0)
            .voltage;
    DOUBLES_EQUAL(5.2, voltage, 0.5);
}

TEST(TcomCheckout, CanSenseCommsRegulator2OutputVoltage) {
    double voltage =
        measurable_manager->ReadNanopbMeasurable<VoltageReading>(kComOutV2, 0)
            .voltage;
    DOUBLES_EQUAL(8.2, voltage, 0.5);
}

TEST(TcomCheckout, CanSenseCommsRegulator2InputVoltage) {
    double voltage =
        measurable_manager->ReadNanopbMeasurable<VoltageReading>(kComInV2, 0)
            .voltage;
    DOUBLES_EQUAL(5.2, voltage, 0.5);
}

TEST(TcomCheckout, CanSenseCommsRegulator1InputCurrent) {
    double current =
        measurable_manager->ReadNanopbMeasurable<CurrentReading>(kComInI1, 0)
            .current;
    DOUBLES_EQUAL(1.5, current, 1.5);
}

TEST(TcomCheckout, CanSenseCommsRegulator1OutputCurrent) {
    double current =
        measurable_manager->ReadNanopbMeasurable<CurrentReading>(kComInI1, 0)
            .current;
    DOUBLES_EQUAL(1.5, current, 1.5);
}

TEST(TcomCheckout, CanSenseCommsRegulator2InputCurrent) {
    double current =
        measurable_manager->ReadNanopbMeasurable<CurrentReading>(kComInI1, 0)
            .current;
    DOUBLES_EQUAL(1.5, current, 1.5);
}

TEST(TcomCheckout, CanSenseCommsRegulator2OutputCurrent) {
    double current =
        measurable_manager->ReadNanopbMeasurable<CurrentReading>(kComOutI2, 0)
            .current;
    DOUBLES_EQUAL(1.5, current, 1.5);
}
