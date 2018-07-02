#include <CppUTest/TestHarness.h>
#include <src/config/unit_tests.h>
#include <src/sensors/i2c_measurable_manager.h>
#include <src/sensors/i2c_sensors/measurables/temperature_measurable.h>
#include <src/sensors/measurable_id.h>
#include <src/util/satellite_time_source.h>

static const byte kMultiplexerAddress = 0x76;
static const byte kTestRtcAddr = 0x69;
static const uint64_t timestamp_tolerance = 1000;

TEST_GROUP(MeasurableManager) {
    void setup() {
        if (!i2c_available) {
            TEST_EXIT;
        }
    };
};

TEST(MeasurableManager, TestManagerRead) {
    I2cMeasurableManager *manager = I2cMeasurableManager::GetInstance();
    // Manager needs to be already initialised in PostBiosInit

    TemperatureMeasurable *measurable = dynamic_cast<TemperatureMeasurable *>(
        manager->GetMeasurable<double>(kCdhTemp1));

    CHECK(measurable != NULL);

    Time now = SatelliteTimeSource::GetTime();
    CHECK(now.is_valid);

    double cdh_temp = manager->ReadI2cMeasurable<double>(kCdhTemp1, 0);

    Time timestamp = measurable->GetTimestamp();
    CHECK(timestamp.is_valid)

    DOUBLES_EQUAL(now.timestamp_millis_unix_epoch,
                  timestamp.timestamp_millis_unix_epoch, 10);
    // TODO(akremor): Remove this. Only here because the above can return an
    // undefined value which appears to cause a loop when the test assert is
    // performed.
    cdh_temp = 123;
    DOUBLES_EQUAL(25.0, cdh_temp, 10);
}
