#include <Board.h>
#include <external/etl/array.h>
#include <src/config/unit_tests.h>
#include <src/i2c/i2c.h>
#include <src/i2c/multiplexers/i2c_multiplexer.h>
#include <test_runners/integration_i2c_tests.h>
#include <test_runners/unity.h>
#include <ti/sysbios/knl/Task.h>
#include <xdc/runtime/Log.h>
#include <xdc/runtime/System.h>
#include <xdc/std.h>

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
        System_printf("Operating on Bus A channel %d\n", channel);
        for (uint8_t addr = 8; addr < 120; addr++) {
            if (bus_a.PerformWriteTransaction(addr, data, 1)) {
                System_printf("Found device at 0x%02x\n", addr);
            }
        }
    }

    System_printf("Operating on Bus B\n");
    for (uint8_t addr = 8; addr < 120; addr++) {
        if (bus_b.PerformWriteTransaction(addr, data, 1)) {
            System_printf("Found device at 0x%02x\n", addr);
        }
    }

    for (uint8_t channel = 0; channel < 8; channel++) {
        bus_c_multiplexer.CloseAllChannels();
        bus_c_multiplexer.OpenChannel(channel);
        System_printf("Operating on Bus C channel %d\n", channel);
        for (uint8_t addr = 8; addr < 120; addr++) {
            if (bus_a.PerformWriteTransaction(addr, data, 1)) {
                System_printf("Found device at 0x%02x\n", addr);
            }
        }
    }

    System_printf("Operating on Bus D\n");
    for (uint8_t addr = 8; addr < 120; addr++) {
        if (bus_d.PerformWriteTransaction(addr, data, 1)) {
            System_printf("Found device at 0x%02x\n", addr);
        }
    }
}
