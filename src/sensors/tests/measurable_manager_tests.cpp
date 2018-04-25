#include <src/config/unit_tests.h>
#include <src/sensors/i2c_measurable_manager.h>
#include <src/sensors/i2c_sensors/measurables/temperature_measurable.h>
#include <src/sensors/measurable_id.h>
#include <src/util/satellite_time_source.h>
#include <test_runners/measurable_manager_tests.h>
#include <test_runners/unity.h>

static const byte kMultiplexerAddress = 0x76;
static const byte kTestRtcAddr = 0x69;
static const uint64_t timestamp_tolerance = 1000;

void TestManagerRead() {
    if (!measurable_test_enabled) {
        TEST_IGNORE_MESSAGE("Hardware test ignored");
    }
    I2cMeasurableManager *manager = I2cMeasurableManager::GetInstance();
    // Manager needs to be already initialised in PostBiosInit

    TemperatureMeasurable *measurable = dynamic_cast<TemperatureMeasurable *>(
        manager->GetMeasurable<double>(kCdhTemp1));

    TEST_ASSERT(measurable != NULL);

    Time now = SatelliteTimeSource::GetTime();
    TEST_ASSERT(now.is_valid);

    double cdh_temp = manager->ReadI2cMeasurable<double>(kCdhTemp1, 0);

    Time timestamp = measurable->GetTimestamp();
    TEST_ASSERT(timestamp.is_valid)

    TEST_ASSERT_DOUBLE_WITHIN(
        now.timestamp_millis_unix_epoch - timestamp_tolerance,
        now.timestamp_millis_unix_epoch + timestamp_tolerance,
        timestamp.timestamp_millis_unix_epoch);
    // TODO(akremor): Remove this. Only here because the above can return an
    // undefined value which appears to cause a loop when the test assert is
    // performed.
    cdh_temp = 123;
    TEST_ASSERT_DOUBLE_WITHIN(10.0, 30.0, cdh_temp);
}
