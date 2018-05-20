#include <src/board/board.h>
#include <src/config/unit_tests.h>
#include <src/messages/antenna_message.h>
#include <src/telecomms/antenna.h>
#include <test_runners/antenna_tests.h>
#include <test_runners/unity.h>

// Tests defined in Arduino code at:
// https://github.com/MelbourneSpaceProgram/hardware_unit_test_software

static const byte kTestHardwareAddr = 0x33;

void TestSafeDeploy(void) {
    if (!antenna_test_enabled) {
        TEST_IGNORE_MESSAGE("Hardware test ignored");
    }
    static uint8_t test_count = 2;
    byte initial_state = 0x00;
    byte expected_final_state = 0xFF;
    byte test_number[2] = {0xF0, 0xF1};
    byte reset = 0xff;

    Antenna *antenna = Antenna::GetAntenna();
    AntennaMessage initial_status(antenna->GetStatus());
    TEST_ASSERT_EQUAL_UINT8(initial_state, initial_status.GetAntennaState());

    // Tests both algorithm 1 and 2
    for (uint8_t i = 0; i < test_count; i++) {
        // Set test on antenna deployment simulator
        antenna->GetBus()->PerformWriteTransaction(kTestHardwareAddr,
                                                   &test_number[i], 1);

        antenna->SafeDeploy();
        TEST_ASSERT(antenna->IsDoorsOpen());

        // Reset antenna deployment simulator
        antenna->GetBus()->PerformWriteTransaction(kTestHardwareAddr, &reset,
                                                   1);
    }
}

void TestForceDeploy(void) {
    if (!antenna_test_enabled) {
        TEST_IGNORE_MESSAGE("Hardware test ignored");
    }
    Antenna *antenna = Antenna::GetAntenna();
    byte test_number = 0xF2;
    byte reset = 0xff;

    // Start primary override test
    antenna->GetBus()->PerformWriteTransaction(kTestHardwareAddr, &test_number,
                                               1);
    antenna->ForceDeploy();
    TEST_ASSERT(antenna->IsDoorsOpen());

    // Reset antenna deployment simulator
    antenna->GetBus()->PerformWriteTransaction(kTestHardwareAddr, &reset, 1);

    // Start backup override test
    test_number = 0xF3;
    antenna->GetBus()->PerformWriteTransaction(kTestHardwareAddr, &test_number,
                                               1);
    antenna->ForceDeploy();
    TEST_ASSERT(antenna->IsDoorsOpen());

    // Reset antenna deployment simulator
    antenna->GetBus()->PerformWriteTransaction(kTestHardwareAddr, &reset, 1);
}
