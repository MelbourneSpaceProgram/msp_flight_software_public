#include <src/board/board.h>
#include <src/board/i2c/i2c.h>
#include <src/board/i2c/multiplexers/i2c_multiplexer.h>
#include <src/config/unit_tests.h>
#include <test_runners/unity.h>
#include <xdc/runtime/Log.h>

void TestEnumerateAllI2cDevices(void) {
    if (!i2c_enumeration_enabled) {
        TEST_IGNORE_MESSAGE("Hardware test ignored");
    }
    uint8_t data[1];
    data[0] = 0x01;

    // Test results should be compared with
    // https://www.lucidchart.com/documents/edit/82fe4bce-67ed-41b5-bcf5-2b20b4187bab/0

    I2c bus_a(I2C_BUS_A);
    I2c bus_b(I2C_BUS_B);
    I2c bus_c(I2C_BUS_C);
    I2c bus_d(I2C_BUS_D);
    I2cMultiplexer bus_a_multiplexer(&bus_a, 0x76);
    I2cMultiplexer bus_c_multiplexer(&bus_c, 0x71);

    for (uint8_t channel = 0; channel < 4; channel++) {
        bus_a_multiplexer.CloseAllChannels();
        bus_a_multiplexer.OpenChannel(channel);
        Log_info1("Operating on Bus A channel %d\n", channel);
        for (uint8_t addr = 8; addr < 120; addr++) {
            // Don't communicate with the multiplexer as it can cause a bus
            // switch
            if (addr == 0x76) {
                continue;
            }
            if (bus_a.PerformWriteTransaction(addr, data, 1)) {
                Log_info1("Found device at 0x%02x\n", addr);
            }
        }
    }

    Log_info0("Operating on Bus B\n");
    for (uint8_t addr = 8; addr < 120; addr++) {
        if (bus_b.PerformWriteTransaction(addr, data, 1)) {
            Log_info1("Found device at 0x%02x\n", addr);
        }
    }

    for (uint8_t channel = 0; channel < 8; channel++) {
        bus_c_multiplexer.CloseAllChannels();
        bus_c_multiplexer.OpenChannel(channel);
        Log_info1("Operating on Bus C channel %d\n", channel);
        for (uint8_t addr = 8; addr < 120; addr++) {
            // Don't communicate with the multiplexer as it can cause a bus
            // switch
            if (addr == 0x71) {
                continue;
            }
            if (bus_c.PerformWriteTransaction(addr, data, 1)) {
                Log_info1("Found device at 0x%02x\n", addr);
            }
        }
    }

    Log_info0("Operating on Bus D\n");
    for (uint8_t addr = 8; addr < 120; addr++) {
        if (bus_d.PerformWriteTransaction(addr, data, 1)) {
            Log_info1("Found device at 0x%02x\n", addr);
        }
    }
}
