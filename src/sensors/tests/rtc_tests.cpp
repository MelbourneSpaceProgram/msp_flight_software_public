#include <src/board/board.h>
#include <src/config/unit_tests.h>
#include <src/i2c/multiplexers/i2c_multiplexer.h>
#include <src/sensors/i2c_sensors/rtc.h>
#include <test_runners/rtc_tests.h>
#include <test_runners/unity.h>
#include <time.h>

static const byte kMultiplexerAddress = 0x76;
static const byte kTestRtcAddr = 0x69;

void TestRtcReadTransaction(void) {
    if (!rtc_test_enabled) {
        TEST_IGNORE_MESSAGE("Hardware test ignored");
    }
    I2c test_i2c_bus(I2C_BUS_A);
    I2cMultiplexer multiplexer(&test_i2c_bus, kMultiplexerAddress);

    multiplexer.OpenChannel(I2cMultiplexer::kMuxChannel0);

    Rtc test_rtc(&test_i2c_bus, kTestRtcAddr);
    RTime time = test_rtc.GetTime();

    multiplexer.CloseAllChannels();
    TEST_ASSERT(test_rtc.ValidTime(time));
}
