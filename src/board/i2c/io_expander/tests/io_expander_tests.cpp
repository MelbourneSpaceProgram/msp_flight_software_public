#include <CppUTest/TestHarness.h>
#include <src/board/board.h>
#include <src/board/i2c/i2c.h>
#include <src/board/i2c/io_expander/io_expander.h>
#include <src/board/i2c/multiplexers/i2c_multiplexer.h>
#include <src/config/unit_tests.h>
#include <src/util/data_types.h>

static const byte kIoExpanderAddress = 0x20;

TEST_GROUP(IoExpander) {
    void setup() {
        if (!i2c_available) {
            TEST_EXIT;
        }
    };
};

// Test RangeSetDirection with direction set to input
TEST(IoExpander, TestRangeSetDirectionInput) {
    // Pins being tested
    I2cIoExpander::IoPin first_pin = I2cIoExpander::kIoPin0;
    I2cIoExpander::IoPin second_pin = I2cIoExpander::kIoPin1;

    I2c bus(I2C_BUS_D);
    I2cIoExpander test_io_expander(&bus, kIoExpanderAddress);

    // Testing
    test_io_expander.RangeSetDirection(first_pin, second_pin,
                                       I2cIoExpander::kIoInput);
    I2cIoExpander::IoDirection read_direction_1 =
        test_io_expander.GetDirection(first_pin);
    I2cIoExpander::IoDirection read_direction_2 =
        test_io_expander.GetDirection(second_pin);

    // Check that the read directions are set to input
    CHECK_TEXT(read_direction_1 == I2cIoExpander::kIoInput &&
                   read_direction_2 == I2cIoExpander::kIoInput,
               "RangeSetDirection not setting input pins to input.");
}

// Test RangeSetDirection with direction set to output
TEST(IoExpander, TestRangeSetDirectionOutput) {
    // Pins being tested
    I2cIoExpander::IoPin first_pin = I2cIoExpander::kIoPin0;
    I2cIoExpander::IoPin second_pin = I2cIoExpander::kIoPin1;

    I2c bus(I2C_BUS_D);
    I2cIoExpander test_io_expander(&bus, kIoExpanderAddress);

    // Testing
    test_io_expander.RangeSetDirection(first_pin, second_pin,
                                       I2cIoExpander::kIoOutput);
    I2cIoExpander::IoDirection read_direction_1 =
        test_io_expander.GetDirection(first_pin);
    I2cIoExpander::IoDirection read_direction_2 =
        test_io_expander.GetDirection(second_pin);

    // Check that the read directions are set to output
    CHECK_TEXT(read_direction_1 == I2cIoExpander::kIoOutput &&
                   read_direction_2 == I2cIoExpander::kIoOutput,
               "RangeSetDirection not setting input pins to output.");
}

TEST(IoExpander, TestRangeIoPinsToInt) {
    // Pins being tested
    I2cIoExpander::IoPin first_pin = I2cIoExpander::kIoPin0;
    I2cIoExpander::IoPin second_pin = I2cIoExpander::kIoPin1;

    I2c bus(I2C_BUS_D);
    I2cIoExpander test_io_expander(&bus, kIoExpanderAddress);

    // Testing
    uint16_t expected = 2;

    test_io_expander.RangeSetDirection(first_pin, second_pin,
                                       I2cIoExpander::kIoOutput);
    test_io_expander.SetPin(first_pin, false);
    test_io_expander.SetPin(second_pin, true);
    uint16_t actual = test_io_expander.RangeIoPinsToInt(first_pin, second_pin);

    CHECK_EQUAL_TEXT(
        expected, actual,
        "RangeIoPinsToInt not returning correct int representation.");
}

// Testing RangeIoPinsToInt again, with an actual output != expected
TEST(IoExpander, TestRangeIoPinsToInt2) {
    // Pins being tested
    I2cIoExpander::IoPin first_pin = I2cIoExpander::kIoPin0;
    I2cIoExpander::IoPin second_pin = I2cIoExpander::kIoPin1;

    I2c bus(I2C_BUS_D);
    I2cIoExpander test_io_expander(&bus, kIoExpanderAddress);

    // Testing
    uint16_t expected = 2;

    // using an actual output of 3 (i.e. 11 in binary)
    test_io_expander.RangeSetDirection(first_pin, second_pin,
                                       I2cIoExpander::kIoOutput);
    test_io_expander.SetPin(first_pin, true);
    test_io_expander.SetPin(second_pin, true);
    uint16_t output = test_io_expander.RangeIoPinsToInt(first_pin, second_pin);

    CHECK_TEXT(output != expected,
               "RangeIoPinsToInt returning incorrect int representation.");
}

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
