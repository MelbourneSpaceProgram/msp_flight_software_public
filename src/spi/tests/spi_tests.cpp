#include <Board.h>
#include <src/config/unit_tests.h>
#include <src/spi/spi.h>
#include <test_runners/spi_tests.h>
#include <test_runners/unity.h>
#include <ti/sysbios/knl/Task.h>

void TestSpiWriteTransaction(void) {
#ifdef SPI_TESTS_IGNORED
    TEST_IGNORE_MESSAGE("Hardware test ignored");
#endif
    for (uint32_t i = 0; i < 8; i++) {
        byte write_buffer = i;
        uint32_t write_buffer_len = 1;
        TEST_ASSERT(Spi::GetInstance()->PerformWriteTransaction(
            10, &write_buffer, write_buffer_len));
    }
}

void TestSpiReadTransaction(void) {
#ifdef SPI_TESTS_IGNORED
    TEST_IGNORE_MESSAGE("Hardware test ignored");
#endif
    for (uint32_t i = 0; i < 8; i++) {
        byte read_buffer = i;
        uint32_t read_buffer_len = 1;
        TEST_ASSERT(Spi::GetInstance()->PerformReadTransaction(
            10, &read_buffer, read_buffer_len));
    }
}
