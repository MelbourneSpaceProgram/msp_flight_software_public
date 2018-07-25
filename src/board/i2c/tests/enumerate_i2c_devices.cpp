#include <CppUTest/TestHarness.h>
#include <src/board/board.h>
#include <src/board/i2c/i2c.h>
#include <src/board/i2c/multiplexers/i2c_multiplexer.h>
#include <src/config/unit_tests.h>
#include <stdio.h>

static void CheckForDevices(I2c& bus, I2cMultiplexer* multiplexer,
                            I2cMultiplexer::MuxChannel channel, byte* devices,
                            int32_t devices_length) {
    if (multiplexer != NULL) {
        multiplexer->CloseAllChannels();
        multiplexer->OpenChannel(channel);
    }
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
        if (!i2c_available || !enumerate_i2c_devices) {
            TEST_EXIT;
        }
    };
};

TEST(I2cDeviceEnumeration, checkBusA_CDH) {
    I2c bus_a(I2C_BUS_A);
    CHECK(bus_a.GetHandle() != NULL);
    I2cMultiplexer bus_a_multiplexer(&bus_a, 0x76);
    byte expected_addresses[] = {0x1A, 0x69};
    CheckForDevices(bus_a, &bus_a_multiplexer, I2cMultiplexer::kMuxChannel0,
                    expected_addresses, sizeof(expected_addresses));
};

TEST(I2cDeviceEnumeration, checkBusA_FS) {
    I2c bus_a(I2C_BUS_A);
    CHECK(bus_a.GetHandle() != NULL);
    I2cMultiplexer bus_a_multiplexer(&bus_a, 0x76);
    byte expected_addresses[] = {0x18, 0x19, 0x1A, 0x68, 0x48, 0x49, 0x4B};
    CheckForDevices(bus_a, &bus_a_multiplexer, I2cMultiplexer::kMuxChannel1,
                    expected_addresses, sizeof(expected_addresses));
};

TEST(I2cDeviceEnumeration, checkBusA_EPS) {
    I2c bus_a(I2C_BUS_A);
    CHECK(bus_a.GetHandle() != NULL);
    I2cMultiplexer bus_a_multiplexer(&bus_a, 0x76);
    byte expected_addresses[] = {0x18, 0x19, 0x48, 0x49, 0x4A};
    CheckForDevices(bus_a, &bus_a_multiplexer, I2cMultiplexer::kMuxChannel2,
                    expected_addresses, sizeof(expected_addresses));
};

TEST(I2cDeviceEnumeration, checkBusA_COM) {
    I2c bus_a(I2C_BUS_A);
    CHECK(bus_a.GetHandle() != NULL);
    I2cMultiplexer bus_a_multiplexer(&bus_a, 0x76);
    byte expected_addresses[] = {0x18, 0x19, 0x48, 0x49, 0x4A};
    CheckForDevices(bus_a, &bus_a_multiplexer, I2cMultiplexer::kMuxChannel3,
                    expected_addresses, sizeof(expected_addresses));
};

TEST(I2cDeviceEnumeration, checkBusB_FS) {
    I2c bus(I2C_BUS_B);
    CHECK(bus.GetHandle() != NULL);
    byte expected_addresses[] = {0x20, 0x68};
    CheckForDevices(bus, NULL, I2cMultiplexer::kMuxNoChannel,
                    expected_addresses, sizeof(expected_addresses));
};

TEST(I2cDeviceEnumeration, checkBusC_FS) {
    I2c bus(I2C_BUS_C);
    CHECK(bus.GetHandle() != NULL);
    I2cMultiplexer multiplexer(&bus, 0x71);
    byte expected_addresses[] = {0x1C, 0x49};
    CheckForDevices(bus, &multiplexer, I2cMultiplexer::kMuxChannel1,
                    expected_addresses, sizeof(expected_addresses));
};

TEST(I2cDeviceEnumeration, checkBusC_EPS) {
    I2c bus(I2C_BUS_C);
    CHECK(bus.GetHandle() != NULL);
    byte expected_addresses[] = {0x68};
    CheckForDevices(bus, NULL, I2cMultiplexer::kMuxNoChannel,
                    expected_addresses, sizeof(expected_addresses));
};

TEST(I2cDeviceEnumeration, checkBusC_UTIL) {
    I2c bus(I2C_BUS_C);
    CHECK(bus.GetHandle() != NULL);
    I2cMultiplexer multiplexer(&bus, 0x71);
    byte expected_addresses[] = {0x49};
    CheckForDevices(bus, &multiplexer, I2cMultiplexer::kMuxChannel1,
                    expected_addresses, sizeof(expected_addresses));
};

TEST(I2cDeviceEnumeration, checkBusC_Panel1) {
    I2c bus(I2C_BUS_C);
    CHECK(bus.GetHandle() != NULL);
    I2cMultiplexer multiplexer(&bus, 0x71);
    byte expected_addresses[] = {0x19, 0x1A, 0x48};
    CheckForDevices(bus, &multiplexer, I2cMultiplexer::kMuxChannel4,
                    expected_addresses, sizeof(expected_addresses));
};

TEST(I2cDeviceEnumeration, checkBusC_Panel2) {
    I2c bus(I2C_BUS_C);
    CHECK(bus.GetHandle() != NULL);
    I2cMultiplexer multiplexer(&bus, 0x71);
    byte expected_addresses[] = {0x19, 0x1A, 0x48};
    CheckForDevices(bus, &multiplexer, I2cMultiplexer::kMuxChannel5,
                    expected_addresses, sizeof(expected_addresses));
};

TEST(I2cDeviceEnumeration, checkBusC_Panel3) {
    I2c bus(I2C_BUS_C);
    CHECK(bus.GetHandle() != NULL);
    I2cMultiplexer multiplexer(&bus, 0x71);
    byte expected_addresses[] = {0x19, 0x1A, 0x48};
    CheckForDevices(bus, &multiplexer, I2cMultiplexer::kMuxChannel6,
                    expected_addresses, sizeof(expected_addresses));
};

TEST(I2cDeviceEnumeration, checkBusC_Panel4) {
    I2c bus(I2C_BUS_C);
    CHECK(bus.GetHandle() != NULL);
    I2cMultiplexer multiplexer(&bus, 0x71);
    byte expected_addresses[] = {0x19, 0x1A, 0x48};
    CheckForDevices(bus, &multiplexer, I2cMultiplexer::kMuxChannel7,
                    expected_addresses, sizeof(expected_addresses));
};

TEST(I2cDeviceEnumeration, checkBusC_Panel5) {
    I2c bus(I2C_BUS_C);
    CHECK(bus.GetHandle() != NULL);
    I2cMultiplexer multiplexer(&bus, 0x71);
    byte expected_addresses[] = {0x19, 0x1A, 0x48};
    CheckForDevices(bus, &multiplexer, I2cMultiplexer::kMuxChannel3,
                    expected_addresses, sizeof(expected_addresses));
};

TEST(I2cDeviceEnumeration, checkBusC_Panel6) {
    I2c bus(I2C_BUS_C);
    CHECK(bus.GetHandle() != NULL);
    I2cMultiplexer multiplexer(&bus, 0x71);
    byte expected_addresses[] = {0x19, 0x4B};
    CheckForDevices(bus, &multiplexer, I2cMultiplexer::kMuxChannel2,
                    expected_addresses, sizeof(expected_addresses));
};
