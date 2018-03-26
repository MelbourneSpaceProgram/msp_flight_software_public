#include <Board.h>
#include <src/config/unit_tests.h>
#include <src/sensors/i2c_sensors/rtc.h>
#include <test_runners/rtc_tests.h>
#include <test_runners/unity.h>
#include <time.h>

static const byte kTestRtcAddr = 0x68;

void TestRtcReadTransaction(void) {
    if (!rtc_test_enabled) {
        TEST_IGNORE_MESSAGE("Hardware test ignored");
    }
    I2c test_i2c_bus(Board_I2C_TMP);
    Rtc test_rtc(&test_i2c_bus, kTestRtcAddr);

    bool success = test_rtc.TakeReading();

    TEST_ASSERT(success);
}
