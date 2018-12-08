#include <CppUTest/TestHarness.h>
#include <src/board/board.h>
#include <src/board/i2c/i2c.h>
#include <src/board/i2c/multiplexers/i2c_multiplexer.h>
#include <src/config/unit_tests.h>
#include <src/sensors/i2c_sensors/mlx90615.h>
#include <src/util/msp_exception.h>

static constexpr uint8_t test_mlx_address = 0x5b;
static constexpr byte panel_mux_address = 0x71;
static constexpr I2cMultiplexer::MuxChannel x_pos_mux_channel =
    I2cMultiplexer::kMuxChannel4;

TEST_GROUP(IrSensor) {
    void setup() {
        if (!SystemConfiguration::GetInstance()->IsI2cAvailable() || !SystemConfiguration::GetInstance()->IsXPosAvailable()) {
            TEST_EXIT;
        }
    };
};

TEST(IrSensor, TestIrRead) {
    I2c test_i2c_bus(I2C_BUS_C);
    I2cMultiplexer multiplexer(&test_i2c_bus, panel_mux_address);

    Mlx90615 mlx90615(&test_i2c_bus, test_mlx_address);

    try {
        multiplexer.OpenChannel(x_pos_mux_channel);
        float read_ir = mlx90615.ReadInfrared();
        float read_dev_temp = mlx90615.ReadDeviceTemperature();
        multiplexer.CloseAllChannels();

        CHECK(-50 < read_ir && read_ir < 150.0);
        CHECK(-50 < read_dev_temp && read_dev_temp < 150.0);
    } catch (MspException &e) {
        FAIL("Uncaught exception in test");
    }
}
