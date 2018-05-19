#include <Board.h>
#include <src/config/unit_tests.h>
#include <src/i2c/i2c.h>
#include <src/i2c/multiplexers/i2c_multiplexer.h>
#include <src/sensors/i2c_sensors/current_sensors/ina211.h>
#include <src/sensors/i2c_sensors/measurables/ina211_current_measurable.h>
#include <test_runners/ina211_tests.h>
#include <test_runners/unity.h>

static const double kRsResistance = 3000;
static const double kVref = 0;

void TestIna211CurrentRead() {
    if (!current_sensor_test_enabled) {
        TEST_IGNORE_MESSAGE("Hardware test ignored");
    }
    I2c bus_a(I2C_BUS_A);
    I2cMultiplexer mux_a(&bus_a, 0x76);
    Adc adc(&bus_a, 0x48, &mux_a, I2cMultiplexer::kMuxChannel0);
    Ina211 current_sensor(&adc, kAdcP0NGnd, kRsResistance, kVref);
    Ina211CurrentMeasurable current_measurable(&current_sensor);
    TEST_ASSERT(current_measurable.TakeReading());
}
