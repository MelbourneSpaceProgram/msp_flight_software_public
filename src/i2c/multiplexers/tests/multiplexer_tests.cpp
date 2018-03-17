#include <Board.h>
#include <src/config/unit_tests.h>
#include <src/i2c/i2c.h>
#include <src/i2c/multiplexers/i2c_multiplexer.h>
#include <src/util/data_types.h>
#include <test_runners/i2c_tests.h>
#include <test_runners/unity.h>
#include <ti/sysbios/knl/Task.h>
#include <src/sensors/i2c_sensors/mcp9808.hpp>

static const byte kMultiplexerAddress = 0x74;
static const byte kTempSensorAddress = 0x18;
static const byte kAdcAddress = 0x48;
static const byte kAdcConversionRegisterLocation = 0x00;
static const double kRoomTemperature = 25;
static const double kAllowableError = 10;

void TestMultiplexer(void) {
#ifdef MULTIPLEXER_TEST_IGNORED
    TEST_IGNORE_MESSAGE("Hardware test ignored");
#endif
    I2c bus(Board_I2C_TMP);
    I2cMultiplexer multiplexer(&bus, kMultiplexerAddress);
    MCP9808 temp_sensor(&bus, kTempSensorAddress, "temp sensor");
    MCP9808 temp_sensor2(&bus, kTempSensorAddress, "temp sensor");

    multiplexer.OpenChannel(I2cMultiplexer::kMuxChannel0);
    TEST_ASSERT(temp_sensor.TakeReading());

    multiplexer.CloseAllChannels();

    // Since all channels are closed the reading should fail
    TEST_ASSERT_FALSE(temp_sensor2.TakeReading());
}
