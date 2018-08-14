#include <CppUTest/TestHarness.h>
#include <src/board/board.h>
#include <src/board/i2c/i2c.h>
#include <src/board/i2c/multiplexers/i2c_multiplexer.h>
#include <src/config/unit_tests.h>
#include <src/sensors/i2c_sensors/mcp9808.h>
#include <src/sensors/i2c_sensors/measurables/temperature_measurable.h>
#include <src/util/data_types.h>
#include <xdc/runtime/Log.h>

static const byte kMultiplexerAddress = 0x76;
static const byte kTempSensorAddress = 0x1A;

TEST_GROUP(Multiplexer) {
    void setup() {
        if (!i2c_available) {
            TEST_EXIT;
        }
    };
};

TEST(Multiplexer, TestMultiplexer) {
    I2c bus(I2C_BUS_A);
    I2cMultiplexer multiplexer(&bus, kMultiplexerAddress);
    MCP9808 temp_sensor(&bus, kTempSensorAddress);
    MCP9808 temp_sensor2(&bus, kTempSensorAddress);

    TemperatureMeasurable accessible_temp(&temp_sensor);
    TemperatureMeasurable inaccessible_temp(&temp_sensor2);

    multiplexer.OpenChannel(I2cMultiplexer::kMuxChannel0);
    CHECK(accessible_temp.TakeReading());
    multiplexer.CloseAllChannels();

    // Since all channels are closed the reading should fail
    Log_info0("An I2c measurable failed read message will be printed. This is expected and okay.");
    CHECK_FALSE(inaccessible_temp.TakeReading());
}
