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
        if (!SystemConfiguration::GetInstance()->IsI2cAvailable()) {
            TEST_EXIT;
        }
    };
};

TEST(I2cDeviceEnumeration, CheckBusACdh) {
    if (!SystemConfiguration::GetInstance()->IsCdhBoardAvailable()) {
        TEST_EXIT;
    }

    I2c bus_a(I2C_BUS_A);
    CHECK(bus_a.GetHandle() != NULL);
    I2cMultiplexer bus_a_multiplexer(&bus_a, 0x76);
    byte expected_addresses[] = {0x1A, 0x69};
    CheckForDevices(bus_a, &bus_a_multiplexer, I2cMultiplexer::kMuxChannel0,
                    expected_addresses, sizeof(expected_addresses));
};

TEST(I2cDeviceEnumeration, CheckBusAFs) {
    if (!SystemConfiguration::GetInstance()->IsFsBoardAvailable()) {
        TEST_EXIT;
    }

    I2c bus_a(I2C_BUS_A);
    CHECK(bus_a.GetHandle() != NULL);
    I2cMultiplexer bus_a_multiplexer(&bus_a, 0x76);
    byte expected_addresses[] = {0x18, 0x19, 0x1A, 0x68, 0x48, 0x49, 0x4B};
    CheckForDevices(bus_a, &bus_a_multiplexer, I2cMultiplexer::kMuxChannel1,
                    expected_addresses, sizeof(expected_addresses));
};

TEST(I2cDeviceEnumeration, CheckBusAEps) {
    if (!SystemConfiguration::GetInstance()->IsEpsBoardAvailable()) {
        TEST_EXIT;
    }

    I2c bus_a(I2C_BUS_A);
    CHECK(bus_a.GetHandle() != NULL);
    I2cMultiplexer bus_a_multiplexer(&bus_a, 0x76);
    byte expected_addresses[] = {0x18, 0x19, 0x48, 0x49, 0x4A};
    CheckForDevices(bus_a, &bus_a_multiplexer, I2cMultiplexer::kMuxChannel2,
                    expected_addresses, sizeof(expected_addresses));
};

TEST(I2cDeviceEnumeration, CheckBusACom) {
    if (!SystemConfiguration::GetInstance()->IsTcomBoardAvailable()) {
        TEST_EXIT;
    }

    I2c bus_a(I2C_BUS_A);
    CHECK(bus_a.GetHandle() != NULL);
    I2cMultiplexer bus_a_multiplexer(&bus_a, 0x76);
    byte expected_addresses[] = {0x18, 0x19, 0x48, 0x49};
    CheckForDevices(bus_a, &bus_a_multiplexer, I2cMultiplexer::kMuxChannel3,
                    expected_addresses, sizeof(expected_addresses));
};

TEST(I2cDeviceEnumeration, CheckBusBFs) {
    if (!SystemConfiguration::GetInstance()->IsFsBoardAvailable()) {
        TEST_EXIT;
    }

    I2c bus(I2C_BUS_B);
    CHECK(bus.GetHandle() != NULL);
    byte expected_addresses[] = {0x20, 0x68};
    CheckForDevices(bus, NULL, I2cMultiplexer::kMuxNoChannel,
                    expected_addresses, sizeof(expected_addresses));
};

TEST(I2cDeviceEnumeration, CheckBusCFs) {
    if (!SystemConfiguration::GetInstance()->IsFsBoardAvailable()) {
        TEST_EXIT;
    }

    I2c bus(I2C_BUS_C);
    CHECK(bus.GetHandle() != NULL);
    I2cMultiplexer multiplexer(&bus, 0x71);
    byte expected_addresses[] = {0x1C, 0x49};
    CheckForDevices(bus, &multiplexer, I2cMultiplexer::kMuxChannel1,
                    expected_addresses, sizeof(expected_addresses));
};

TEST(I2cDeviceEnumeration, CheckBusCEps) {
    if (!SystemConfiguration::GetInstance()->IsEpsBoardAvailable()) {
        TEST_EXIT;
    }

    I2c bus(I2C_BUS_C);
    CHECK(bus.GetHandle() != NULL);
    byte expected_addresses[] = {0x68};
    CheckForDevices(bus, NULL, I2cMultiplexer::kMuxNoChannel,
                    expected_addresses, sizeof(expected_addresses));
};

TEST(I2cDeviceEnumeration, CheckBusDEps) {
    if (!SystemConfiguration::GetInstance()->IsEpsBoardAvailable()) {
        TEST_EXIT;
    }

    I2c bus(I2C_BUS_D);
    CHECK(bus.GetHandle() != NULL);
    byte expected_addresses[] = {0x68};
    CheckForDevices(bus, NULL, I2cMultiplexer::kMuxNoChannel,
                    expected_addresses, sizeof(expected_addresses));
};

TEST(I2cDeviceEnumeration, CheckBusCUtilities) {
    if (!SystemConfiguration::GetInstance()->IsUtilitiesBoardAvailable()) {
        TEST_EXIT;
    }

    I2c bus(I2C_BUS_C);
    CHECK(bus.GetHandle() != NULL);
    I2cMultiplexer multiplexer(&bus, 0x71);
    byte expected_addresses[] = {0x49};
    CheckForDevices(bus, &multiplexer, I2cMultiplexer::kMuxChannel1,
                    expected_addresses, sizeof(expected_addresses));
};

TEST(I2cDeviceEnumeration, CheckBusCXPos) {
    if (!SystemConfiguration::GetInstance()->IsXPosAvailable()) {
        TEST_EXIT;
    }

    I2c bus(I2C_BUS_C);
    CHECK(bus.GetHandle() != NULL);
    I2cMultiplexer multiplexer(&bus, 0x71);
    byte expected_addresses[] = {0x19, 0x1A, 0x29, 0x48};
    CheckForDevices(bus, &multiplexer, I2cMultiplexer::kMuxChannel4,
                    expected_addresses, sizeof(expected_addresses));
};

TEST(I2cDeviceEnumeration, CheckBusCYPos) {
    if (!SystemConfiguration::GetInstance()->IsYPosAvailable()) {
        TEST_EXIT;
    }

    I2c bus(I2C_BUS_C);
    CHECK(bus.GetHandle() != NULL);
    I2cMultiplexer multiplexer(&bus, 0x71);
    byte expected_addresses[] = {0x19, 0x1A, 0x29, 0x48};
    CheckForDevices(bus, &multiplexer, I2cMultiplexer::kMuxChannel5,
                    expected_addresses, sizeof(expected_addresses));
};

TEST(I2cDeviceEnumeration, CheckBusCXNeg) {
    if (!SystemConfiguration::GetInstance()->IsXNegAvailable()) {
        TEST_EXIT;
    }

    I2c bus(I2C_BUS_C);
    CHECK(bus.GetHandle() != NULL);
    I2cMultiplexer multiplexer(&bus, 0x71);
    byte expected_addresses[] = {0x19, 0x1A, 0x29, 0x48};
    CheckForDevices(bus, &multiplexer, I2cMultiplexer::kMuxChannel6,
                    expected_addresses, sizeof(expected_addresses));
};

TEST(I2cDeviceEnumeration, CheckBusCYNeg) {
    if (!SystemConfiguration::GetInstance()->IsYNegAvailable()) {
        TEST_EXIT;
    }

    I2c bus(I2C_BUS_C);
    CHECK(bus.GetHandle() != NULL);
    I2cMultiplexer multiplexer(&bus, 0x71);
    byte expected_addresses[] = {0x19, 0x1A, 0x29, 0x48};
    CheckForDevices(bus, &multiplexer, I2cMultiplexer::kMuxChannel7,
                    expected_addresses, sizeof(expected_addresses));
};

TEST(I2cDeviceEnumeration, CheckBusCZNeg) {
    if (!SystemConfiguration::GetInstance()->IsZNegAvailable()) {
        TEST_EXIT;
    }

    I2c bus(I2C_BUS_C);
    CHECK(bus.GetHandle() != NULL);
    I2cMultiplexer multiplexer(&bus, 0x71);
    byte expected_addresses[] = {0x19, 0x1A, 0x29, 0x48};
    CheckForDevices(bus, &multiplexer, I2cMultiplexer::kMuxChannel3,
                    expected_addresses, sizeof(expected_addresses));
};

TEST(I2cDeviceEnumeration, CheckBusCZPos) {
    if (!SystemConfiguration::GetInstance()->IsZPosAvailable()) {
        TEST_EXIT;
    }

    I2c bus(I2C_BUS_C);
    CHECK(bus.GetHandle() != NULL);
    I2cMultiplexer multiplexer(&bus, 0x71);
    byte expected_addresses[] = {0x19, 0x29, 0x4B};
    CheckForDevices(bus, &multiplexer, I2cMultiplexer::kMuxChannel2,
                    expected_addresses, sizeof(expected_addresses));
};
