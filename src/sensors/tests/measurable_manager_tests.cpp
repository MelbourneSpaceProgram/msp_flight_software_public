#include <CppUTest/TestHarness.h>
#include <src/config/unit_tests.h>
#include <src/messages/TemperatureReading.pb.h>
#include <src/sensors/measurable_manager.h>
#include <src/sensors/i2c_sensors/measurables/temperature_measurable.h>
#include <src/sensors/measurable_id.h>
#include <src/util/satellite_time_source.h>

TEST_GROUP(MeasurableManager) {
    void setup() {
        if (!kI2cAvailable) {
            TEST_EXIT;
        }
    };
};

TEST(MeasurableManager, TestManagerRead) {
    MeasurableManager *manager = MeasurableManager::GetInstance();
    // Manager needs to be already initialised in PostBiosInit

    TemperatureMeasurable *measurable = dynamic_cast<TemperatureMeasurable *>(
        manager->GetMeasurable<TemperatureReading>(kCdhT));

    CHECK(measurable != NULL);

    Time now = SatelliteTimeSource::GetTime();
    CHECK(now.is_valid);

    TemperatureReading cdh_temp =
        manager->ReadNanopbMeasurable<TemperatureReading>(kCdhT, 0);

    CHECK(cdh_temp.timestamp_ms != 0)

    // Relaxed timestamp check within +- 5 seconds
    DOUBLES_EQUAL(now.timestamp_ms, cdh_temp.timestamp_ms, 5000);
    DOUBLES_EQUAL(25, cdh_temp.temp, 10);
}
