#include <CppUTest/TestHarness.h>
#include <src/board/board.h>
#include <src/board/i2c/i2c.h>
#include <src/config/unit_tests.h>

static const byte kTestHardwareAddr = 0x76;

// These tests require the CDH I2C multiplexer to be powered on. It is usually
// powered on in the I2c::InitBusses call.
TEST_GROUP(I2c) {
    void setup() {
        if (!kI2cAvailable) {
            TEST_EXIT;
        }
    };
};

TEST(I2c, TestI2cWriteTransaction) {
    byte write_buffer = 0x00;
    uint16_t write_buffer_len = 1;
    I2c test_i2c_bus(I2C_BUS_A);
    // In effect, this test works by closing all channels on the mux
    // And seeing if the transaction went through
    CHECK(test_i2c_bus.PerformWriteTransaction(kTestHardwareAddr, &write_buffer,
                                               write_buffer_len));
}

TEST(I2c, TestI2cReadTransaction) {
    byte read_buffer;
    uint16_t read_buffer_len = 1;
    I2c test_i2c_bus(I2C_BUS_A);
    CHECK(test_i2c_bus.PerformReadTransaction(kTestHardwareAddr, &read_buffer,
                                              read_buffer_len));
}
