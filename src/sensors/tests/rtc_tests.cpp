#include <CppUTest/TestHarness.h>
#include <src/board/board.h>
#include <src/board/i2c/i2c.h>
#include <src/board/i2c/multiplexers/i2c_multiplexer.h>
#include <src/config/unit_tests.h>
#include <src/sensors/i2c_sensors/rtc.h>
#include <time.h>

static const byte kMultiplexerAddress = 0x76;
static const byte kTestRtcAddr = 0x69;

TEST_GROUP(Rtc) {
    void setup() {
        if (!i2c_available) {
            TEST_EXIT;
        }
    };
};

TEST(Rtc, TestRtcReadTransaction) {
    I2c test_i2c_bus(I2C_BUS_A);
    I2cMultiplexer multiplexer(&test_i2c_bus, kMultiplexerAddress);

    multiplexer.OpenChannel(I2cMultiplexer::kMuxChannel0);

    Rtc test_rtc(&test_i2c_bus, kTestRtcAddr);
    RTime time = test_rtc.GetTime();

    multiplexer.CloseAllChannels();
    CHECK(test_rtc.ValidTime(time));
}
