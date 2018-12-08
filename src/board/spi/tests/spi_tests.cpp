#include <CppUTest/TestHarness.h>
#include <src/board/board.h>
#include <src/board/spi/spi.h>
#include <src/config/unit_tests.h>

TEST_GROUP(Spi) {
    // TODO(dingbenjamin): SD card functionality 'hogs' the SPI bus causing all
    // other SPI transactions to fail. This conditional can be removed after an
    // SPI bus manager is created
    void setup() {
        if (SystemConfiguration::GetInstance()->IsSdCardAvailable()) {
            TEST_EXIT;
        }
    };
};

TEST(Spi, TestSpiWriteTransaction) {
    for (uint32_t i = 0; i < 8; i++) {
        byte write_buffer = i;
        uint32_t write_buffer_len = 1;
        CHECK(Spi::GetInstance().PerformWriteTransaction(10, &write_buffer,
                                                         write_buffer_len));
    }
}

TEST(Spi, TestSpiReadTransaction) {
    for (uint32_t i = 0; i < 8; i++) {
        byte read_buffer = i;
        uint32_t read_buffer_len = 1;
        CHECK(Spi::GetInstance().PerformReadTransaction(10, &read_buffer,
                                                        read_buffer_len));
    }
}
