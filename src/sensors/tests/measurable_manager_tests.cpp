#include <CppUTest/TestHarness.h>
#include <src/config/unit_tests.h>
#include <src/sensors/i2c_measurable_manager.h>
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
    I2cMeasurableManager *manager = I2cMeasurableManager::GetInstance();
    // Manager needs to be already initialised in PostBiosInit

    TemperatureMeasurable *measurable = dynamic_cast<TemperatureMeasurable *>(
        manager->GetMeasurable<double>(kCdhT));

    CHECK(measurable != NULL);

    Time now = SatelliteTimeSource::GetTime();
    CHECK(now.is_valid);

    double cdh_temp = manager->ReadI2cMeasurable<double>(kCdhT, 0);

    Time timestamp = measurable->GetTimestamp();
    CHECK(timestamp.is_valid)

    DOUBLES_EQUAL(now.timestamp_millis_unix_epoch,
                  timestamp.timestamp_millis_unix_epoch, 10);
    DOUBLES_EQUAL(25, cdh_temp, 10);
}
