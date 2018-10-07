#include <CppUTest/TestHarness.h>
#include <src/config/unit_tests.h>
#include <src/messages/TemperatureReading.pb.h>
#include <src/sensors/measurable_manager.h>

TEST_GROUP(CdhCheckout) {
    void setup() {
        if (!kCdhBoardAvailable) {
            TEST_EXIT
        }
    };
};

static MeasurableManager* measurable_manager =
    MeasurableManager::GetInstance();

TEST(CdhCheckout, CanSenseSysTemperature) {
    double cdh_t =
        measurable_manager->ReadNanopbMeasurable<TemperatureReading>(kCdhT, 0)
            .temp;
    DOUBLES_EQUAL(20, cdh_t, 45);
}
