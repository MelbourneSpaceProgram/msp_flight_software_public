#include <CppUTest/TestHarness.h>
#include <src/board/board.h>
#include <src/board/i2c/i2c.h>
#include <src/board/i2c/io_expander/io_expander.h>
#include <src/board/i2c/multiplexers/i2c_multiplexer.h>
#include <src/config/unit_tests.h>
#include <src/util/data_types.h>

static const byte kIoExpanderAddress = 0x22;

TEST_GROUP(IoExpander) {
    void setup() {
        if (!i2c_available) {
            TEST_EXIT;
        }
    };
};

// Test disabled as CDH no longer has an IO Expander
// TODO(akremor): Find equivalent chip
IGNORE_TEST(IoExpander, TestIoExpander) {
    // DANGER DANGER:
    //
    // This test affects actual hardware.
    // The pin to test should be a safe, non-essential pin that won't destroy
    // anything expensive.
    //
    // Pin 02 is power shutoff for the secondary microcontroller.
    // It should be fairly safe to read and write this pin for testing.
    I2cIoExpander::IoPin safe_pin = I2cIoExpander::kIoPin2;

    // Open the IO Expander on Bus D, address 0x22
    I2c bus(I2C_BUS_D);
    I2cIoExpander test_io_expander(&bus, kIoExpanderAddress);

    // Perform SetDirection and GetDirection test as Input
    test_io_expander.SetDirection(safe_pin, I2cIoExpander::kIoInput);
    I2cIoExpander::IoDirection read_direction =
        test_io_expander.GetDirection(safe_pin);

    // Check that the read direction is actually input
    CHECK_TEXT(read_direction == I2cIoExpander::kIoInput,
               "GetDirection was not input after SetDirection");

    // TODO(crozone): Perform GetValue test as pin Input

    // Perform SetDirection and GetDirection test as Output
    test_io_expander.SetDirection(safe_pin, I2cIoExpander::kIoOutput);
    read_direction = test_io_expander.GetDirection(safe_pin);

    // Check that the read direction is actually output
    CHECK_TEXT(read_direction == I2cIoExpander::kIoOutput,
               "GetDirection was not input after SetDirection");

    // SetPin and GetPin test as Hi
    test_io_expander.SetPin(safe_pin, true);
    bool read_value = test_io_expander.GetPin(safe_pin);

    CHECK_TEXT(read_value, "GetPin was not Hi after SetPin");

    // SetPin and GetPin test as Lo
    test_io_expander.SetPin(safe_pin, false);
    read_value = test_io_expander.GetPin(safe_pin);

    CHECK_FALSE_TEXT(read_value, "GetPin was not Lo after SetPin");
}
