#include <CppUTest/TestHarness.h>
#include <src/board/board.h>
#include <src/board/i2c/i2c.h>
#include <src/config/unit_tests.h>
#include <src/messages/antenna_message.h>
#include <src/telecomms/antenna.h>

// Tests defined in Arduino code at:
// https://github.com/MelbourneSpaceProgram/hardware_unit_test_software

static const byte kTestHardwareAddr = 0x33;

TEST_GROUP(Antenna) {
    void setup() {
        if (!kAntennaAvailable) {
            TEST_EXIT;
        }
    };
};

TEST(Antenna, TestSafeDeploy) {
    static uint8_t test_count = 2;
    byte initial_state = 0x00;
    byte test_number[2] = {0xF0, 0xF1};
    byte reset = 0xff;

    Antenna *antenna = Antenna::GetAntenna();
    AntennaMessage initial_status(antenna->GetStatus());
    CHECK_EQUAL(initial_state, initial_status.GetAntennaState());

    // Tests both algorithm 1 and 2
    for (uint8_t i = 0; i < test_count; i++) {
        // Set test on antenna deployment simulator
        antenna->GetBus()->PerformWriteTransaction(kTestHardwareAddr,
                                                   &test_number[i], 1);

        antenna->SafeDeploy();
        CHECK(antenna->IsDoorsOpen());

        // Reset antenna deployment simulator
        antenna->GetBus()->PerformWriteTransaction(kTestHardwareAddr, &reset,
                                                   1);
    }
}

TEST(Antenna, TestForceDeploy) {
    Antenna *antenna = Antenna::GetAntenna();
    byte test_number = 0xF2;
    byte reset = 0xff;

    // Start primary override test
    antenna->GetBus()->PerformWriteTransaction(kTestHardwareAddr, &test_number,
                                               1);
    antenna->ForceDeploy();
    CHECK(antenna->IsDoorsOpen());

    // Reset antenna deployment simulator
    antenna->GetBus()->PerformWriteTransaction(kTestHardwareAddr, &reset, 1);

    // Start backup override test
    test_number = 0xF3;
    antenna->GetBus()->PerformWriteTransaction(kTestHardwareAddr, &test_number,
                                               1);
    antenna->ForceDeploy();
    CHECK(antenna->IsDoorsOpen());

    // Reset antenna deployment simulator
    antenna->GetBus()->PerformWriteTransaction(kTestHardwareAddr, &reset, 1);
}
