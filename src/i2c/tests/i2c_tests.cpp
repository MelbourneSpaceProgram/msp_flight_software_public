#include <src/board/board.h>
#include <test_runners/i2c_tests.h>
#include <test_runners/unity.h>
#include <src/i2c/i2c.h>
#include <src/config/unit_tests.h>
#include <ti/sysbios/knl/Task.h>

static const byte kTestHardwareAddr = 0x76;

void TestI2cWriteTransaction(void) {
    if (!i2c_test_enabled) {
        TEST_IGNORE_MESSAGE("Hardware test ignored");
    }
    byte write_buffer = 0x00;
    uint16_t write_buffer_len = 1;
    I2c test_i2c_bus(I2C_BUS_A);
    // In effect, this test works by closing all channels on the mux
    // And seeing if the transaction went through
    TEST_ASSERT(test_i2c_bus.PerformWriteTransaction(
            kTestHardwareAddr, &write_buffer, write_buffer_len));
}

void TestI2cReadTransaction(void) {
    if (!i2c_test_enabled) {
        TEST_IGNORE_MESSAGE("Hardware test ignored");
    }
    byte read_buffer;
    uint16_t read_buffer_len = 1;
    I2c test_i2c_bus(I2C_BUS_A);
    TEST_ASSERT(test_i2c_bus.PerformReadTransaction(
            kTestHardwareAddr, &read_buffer, read_buffer_len));
    // Relies on the state set by the TestI2cWriteTransaction test
    TEST_ASSERT_EQUAL_UINT8(read_buffer, 0x00);
}
