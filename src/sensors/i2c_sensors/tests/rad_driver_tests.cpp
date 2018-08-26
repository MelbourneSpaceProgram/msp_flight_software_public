#include <CppUTest/TestHarness.h>
#include <src/board/board.h>
#include <src/config/unit_tests.h>
#include <src/sensors/i2c_sensors/i2c_device.h>
#include <src/sensors/i2c_sensors/rad_driver.h>

#include <src/sensors/i2c_sensors/adc.h>

/**
 * Data for these tests has been sourced from the "Radiation Sensor Test v3"
 * jama doc (CS1-DD-270).
 *
 * For the exposure / sensor values, the procedure detailed in the doc should be
 * followed (to attain similar data).
 *
 * TODO(daniel632): Get more accurate tolerance values (They're educated
 * guesses)
 *
 */

// Radiation values (counts / second)
static const double kAvgRoomRad = 0.034281;
static const double kRoomRadTolerance = 0.01;
static const double kAvgExposureRad = 0.670214;
static const double kExposureRadTolerance = 0.1;

static const int kRadAddress = 0x20;
static const int kMuxAddress = 0x71;

TEST_GROUP(RadDriver) {
    void setup() {
        if (!i2c_available || !fs_board_available) {
            TEST_EXIT;
        }
    };
};

// Test radiation reading (on bus B) in a normal environment
TEST(RadDriver, TestRoomReadB) {
    I2c test_i2c_bus(I2C_BUS_B);
    RadDriver test_rad_driver(&test_i2c_bus, kRadAddress);

    double rad_reading = test_rad_driver.TakeI2cReading();

    CHECK((rad_reading < kAvgRoomRad + kRoomRadTolerance) &&
          (rad_reading > kAvgRoomRad - kRoomRadTolerance));
}

// Test radiation reading (on bus C) in a normal environment. This is routed
// through the a mux(0x71) on channel 4. Refer the to I2C Architecture doc on
// lucidchart.
/*
 * (24th aug) Solar Panel Rad Sensor [MUX(0x71)]: The external sensor is located
 * on channel number 4
 */
TEST(RadDriver, TestRoomReadC) {
    I2c test_i2c_bus(I2C_BUS_C);
    I2cMultiplexer *mux_c = new I2cMultiplexer(&test_i2c_bus, kMuxAddress);
    RadDriver test_rad_driver(&test_i2c_bus, kRadAddress, mux_c,
                              I2cMultiplexer::kMuxChannel4);
    double rad_reading = test_rad_driver.TakeI2cReading();

    CHECK((rad_reading < kAvgRoomRad + kRoomRadTolerance) &&
          (rad_reading > kAvgRoomRad - kRoomRadTolerance));
}

// Test radiation reading (on bus B), exposing it to a rad source.
TEST(RadDriver, TestRadExposureRead) {
    I2c test_i2c_bus(I2C_BUS_B);
    RadDriver test_rad_driver(&test_i2c_bus, kRadAddress);
    double rad_reading = test_rad_driver.TakeI2cReading();

    CHECK((rad_reading < kAvgExposureRad + kExposureRadTolerance) &&
          (rad_reading > kAvgExposureRad - kExposureRadTolerance));
}
