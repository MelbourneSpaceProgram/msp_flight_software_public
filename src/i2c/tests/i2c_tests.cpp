#include <test_runners/i2c_tests.h>
#include <test_runners/unity.h>
#include <src/i2c/i2c.h>
#include <src/i2c/i2c_configuration.h>
#include <Board.h>
#include <src/config/unit_tests.h>
#include <ti/sysbios/knl/Task.h>

static const byte kTestHardwareAddr = 0x33;

void TestI2cWriteTransaction(void) {
#ifdef I2C_TESTS_IGNORED
    TEST_IGNORE_MESSAGE("Hardware test ignored");
#endif
    for (uint16_t i = 0; i < 256; i++) {
        byte write_buffer = i;
        uint16_t write_buffer_len = 1;
        I2c test_i2c_bus(I2cConfiguration(), Board_I2C0);
        TEST_ASSERT(test_i2c_bus.PerformWriteTransaction(
                kTestHardwareAddr, &write_buffer, write_buffer_len));
        Task_sleep(100);
        // TODO(wschuetz) Remove Task_sleep() once infinite loop bug found
    }
}

void TestI2cReadTransaction(void) {
#ifdef I2C_TESTS_IGNORED
    TEST_IGNORE_MESSAGE("Hardware test ignored");
#endif
    for (uint16_t i = 0; i < 256; i++) {
        byte read_buffer = i;
        uint16_t read_buffer_len = 1;
        I2c test_i2c_bus(I2cConfiguration(), Board_I2C0);
        TEST_ASSERT(test_i2c_bus.PerformReadTransaction(
                kTestHardwareAddr, &read_buffer, read_buffer_len));
        TEST_ASSERT_EQUAL_UINT8(read_buffer, i);
        Task_sleep(100);
        // TODO(wschuetz) Remove Task_sleep() once infinite loop bug found
    }
}
