#include <src/board/board.h>
#include <src/config/unit_tests.h>
#include <src/i2c/i2c.h>
#include <src/i2c/multiplexers/i2c_multiplexer.h>
#include <src/sensors/i2c_sensors/current_sensors/ina169.h>
#include <src/sensors/i2c_sensors/measurables/ina169_current_measurable.h>
#include <test_runners/ina169_tests.h>
#include <test_runners/unity.h>

static const double kRsResistance = 2.2;
static const double kRlResistance = 20000.0;

void TestIna169CurrentRead() {
    if (!current_sensor_test_enabled) {
        TEST_IGNORE_MESSAGE("Hardware test ignored");
    }
    I2c bus_a(I2C_BUS_A);
    I2cMultiplexer mux_a(&bus_a, 0x76);
    Adc adc(&bus_a, 0x49, &mux_a, I2cMultiplexer::kMuxChannel0);
    Ina169 current_sensor(&adc, kAdcP0N1, kRsResistance, kRlResistance);
    Ina169CurrentMeasurable current_measurable(&current_sensor);
    TEST_ASSERT(current_measurable.TakeReading());
}
