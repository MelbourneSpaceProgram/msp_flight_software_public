#include <CppUTest/TestHarness.h>
#include <src/board/board.h>
#include <src/board/i2c/i2c.h>
#include <src/board/i2c/multiplexers/i2c_multiplexer.h>
#include <src/config/unit_tests.h>
#include <stdio.h>

static void findDevices(I2c& bus, I2cMultiplexer& multiplexer,
                        I2cMultiplexer::MuxChannel channel, byte* devices,
                        int32_t devices_length) {
    multiplexer.CloseAllChannels();
    multiplexer.OpenChannel(channel);
    uint8_t data[1] = {0x00};
    for (uint8_t i = 0; i < devices_length; i++) {
        char device_name[33 + 1];
        sprintf(device_name, "Could not detect at address: 0x%x", devices[i]);
        CHECK_TEXT(bus.PerformWriteTransaction(devices[i], data, 1),
                   device_name);
    }
}

TEST_GROUP(I2cDeviceEnumeration) {
    void setup() {
        if (!i2c_available) {
            TEST_EXIT;
        }
    };
};

TEST(I2cDeviceEnumeration, checkBusA_CDH) {
    I2c bus_a(I2C_BUS_A);
    CHECK(bus_a.GetHandle() != NULL);
    I2cMultiplexer bus_a_multiplexer(&bus_a, 0x76);
    byte expected_addresses[] = {0x1A, 0x69};
    findDevices(bus_a, bus_a_multiplexer, I2cMultiplexer::kMuxChannel0,
                expected_addresses, sizeof(expected_addresses));
};

TEST(I2cDeviceEnumeration, checkBusA_FS) {
    I2c bus_a(I2C_BUS_A);
    CHECK(bus_a.GetHandle() != NULL);
    I2cMultiplexer bus_a_multiplexer(&bus_a, 0x76);
    byte expected_addresses[] = {0x18, 0x19, 0x1A, 0x68,
                                 0x48, 0x49, 0x4A, 0x4B};
    findDevices(bus_a, bus_a_multiplexer, I2cMultiplexer::kMuxChannel1,
                expected_addresses, sizeof(expected_addresses));
};

TEST(I2cDeviceEnumeration, checkBusA_EPS) {
    I2c bus_a(I2C_BUS_A);
    CHECK(bus_a.GetHandle() != NULL);
    I2cMultiplexer bus_a_multiplexer(&bus_a, 0x76);
    byte expected_addresses[] = {0x18, 0x19, 0x48, 0x49, 0x4A};
    findDevices(bus_a, bus_a_multiplexer, I2cMultiplexer::kMuxChannel2,
                expected_addresses, sizeof(expected_addresses));
};

TEST(I2cDeviceEnumeration, checkBusA_COM) {
    I2c bus_a(I2C_BUS_A);
    CHECK(bus_a.GetHandle() != NULL);
    I2cMultiplexer bus_a_multiplexer(&bus_a, 0x76);
    byte expected_addresses[] = {0x18, 0x19, 0x48, 0x49, 0x4A};
    findDevices(bus_a, bus_a_multiplexer, I2cMultiplexer::kMuxChannel3,
                expected_addresses, sizeof(expected_addresses));
};
