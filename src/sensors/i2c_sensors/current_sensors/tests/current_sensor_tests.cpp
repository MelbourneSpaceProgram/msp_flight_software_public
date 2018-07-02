#include <CppUTest/TestHarness.h>
#include <src/board/board.h>
#include <src/board/i2c/i2c.h>
#include <src/board/i2c/multiplexers/i2c_multiplexer.h>
#include <src/config/unit_tests.h>
#include <src/sensors/i2c_sensors/current_sensors/ina169.h>
#include <src/sensors/i2c_sensors/current_sensors/ina210.h>
#include <src/sensors/i2c_sensors/current_sensors/ina211.h>
#include <src/sensors/i2c_sensors/current_sensors/ina212.h>
#include <src/sensors/i2c_sensors/measurables/ina169_current_measurable.h>
#include <src/sensors/i2c_sensors/measurables/ina210_current_measurable.h>
#include <src/sensors/i2c_sensors/measurables/ina211_current_measurable.h>
#include <src/sensors/i2c_sensors/measurables/ina212_current_measurable.h>

TEST_GROUP(CurrentSensor){
    void setup(){if (!current_sensor_test_enabled){TEST_EXIT;
}
}
}
;

TEST(CurrentSensor, INA169) {
    double kRsResistance = 2.2;
    double kRlResistance = 20000.0;

    I2c bus_a(I2C_BUS_A);
    I2cMultiplexer mux_a(&bus_a, 0x76);
    Adc adc(&bus_a, 0x49, &mux_a, I2cMultiplexer::kMuxChannel0);
    Ina169 current_sensor(&adc, kAdcP0N1, kRsResistance, kRlResistance);
    Ina169CurrentMeasurable current_measurable(&current_sensor);
    CHECK(current_measurable.TakeReading());
}

TEST(CurrentSensor, INA210) {
    double kRsResistance = 3000;
    double kVref = 0;
    I2c bus_a(I2C_BUS_A);
    I2cMultiplexer mux_a(&bus_a, 0x76);
    Adc adc(&bus_a, 0x48, &mux_a, I2cMultiplexer::kMuxChannel0);
    Ina210 current_sensor(&adc, kAdcP0NGnd, kRsResistance, kVref);
    Ina210CurrentMeasurable current_measurable(&current_sensor);
    CHECK(current_measurable.TakeReading());
}

TEST(CurrentSensor, TestIna211CurrentRead) {
    double kRsResistance = 3000;
    double kVref = 0;
    I2c bus_a(I2C_BUS_A);
    I2cMultiplexer mux_a(&bus_a, 0x76);
    Adc adc(&bus_a, 0x48, &mux_a, I2cMultiplexer::kMuxChannel0);
    Ina211 current_sensor(&adc, kAdcP0NGnd, kRsResistance, kVref);
    Ina211CurrentMeasurable current_measurable(&current_sensor);
    CHECK(current_measurable.TakeReading());
}

TEST(CurrentSensor, TestIna212CurrentRead) {
    double kRsResistance = 3000;
    double kVref = 0;

    I2c bus_a(I2C_BUS_A);
    I2cMultiplexer mux_a(&bus_a, 0x76);
    Adc adc(&bus_a, 0x49, &mux_a, I2cMultiplexer::kMuxChannel0);
    Ina212 current_sensor(&adc, kAdcP0NGnd, kRsResistance, kVref);
    Ina212CurrentMeasurable current_measurable(&current_sensor);
    CHECK(current_measurable.TakeReading());
}
