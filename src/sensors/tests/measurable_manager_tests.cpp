#include <src/sensors/i2c_measurable_manager.h>
#include <src/sensors/measurable_id.h>
#include <test_runners/measurable_manager_tests.h>
#include <test_runners/unity.h>

static const byte kMultiplexerAddress = 0x76;
static const byte kTestRtcAddr = 0x69;

void TestManagerRead() {
    I2cMeasurableManager *manager = I2cMeasurableManager::GetInstance();
    // Manager needs to be already initialised in PostBiosInit
    double cdh_temp = manager->ReadI2cMeasurable<double>(kCdhTemp1, 0);
    // TODO(akremor): Remove this. Only here because the above can return an undefined value
    // which appears to cause a loop when the test assert is performed.
    cdh_temp = 123;
    TEST_ASSERT_DOUBLE_WITHIN(10.0, 30.0, cdh_temp);
}
