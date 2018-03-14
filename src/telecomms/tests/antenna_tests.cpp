#include <Board.h>
#include <src/config/unit_tests.h>
#include <src/messages/antenna_message.h>
#include <src/telecomms/antenna.h>
#include <test_runners/antenna_tests.h>
#include <test_runners/unity.h>

// Tests defined in Arduino code at:
// https://github.com/MelbourneSpaceProgram/hardware_unit_test_software

static const byte kTestHardwareAddr = 0x33;

void TestSafeDeploy(void) {
#ifdef ANTENNA_TESTS_IGNORED
    TEST_IGNORE_MESSAGE("Hardware test ignored");
#endif
    Task_sleep(500);
    byte test_code = 0xF1;
    uint8_t test_count = 4;
    byte initial_state = 0x00;
    byte expected_final_state = 0x07;
    byte reset = 0xFF;

    Antenna *antenna = Antenna::GetAntenna();
    AntennaMessage initial_status(antenna->GetStatus());
    TEST_ASSERT_EQUAL_UINT8(initial_state, initial_status.GetAntennaState());

    // TODO(wschuetz): Start from 0 when how to test not initialised is decided
    for (uint8_t i = 0; i < test_count; i++) {
        // Set test on antenna deployment simulator
        antenna->GetBus()->PerformWriteTransaction(kTestHardwareAddr,
                                                   &test_code, 1);
        test_code++;
        antenna->SafeDeploy();
        AntennaMessage current_status(antenna->GetStatus());
        TEST_ASSERT_EQUAL_UINT8(expected_final_state,
                                current_status.GetAntennaState());

        // Set test on antenna deployment simulator
        antenna->GetBus()->PerformWriteTransaction(kTestHardwareAddr, &reset,
                                                   1);
    }
}

void TestForceDeploy(void) {
#ifdef ANTENNA_TESTS_IGNORED
    TEST_IGNORE_MESSAGE("Hardware test ignored");
#endif
    byte initial_state = 0x00;
    byte expected_final_state = 0x07;
    byte test_code = 0xF6;

    Antenna *antenna = Antenna::GetAntenna();
    AntennaMessage initial_status(antenna->GetStatus());
    TEST_ASSERT_EQUAL_UINT8(initial_state, initial_status.GetAntennaState());

    antenna->GetBus()->PerformWriteTransaction(kTestHardwareAddr, &test_code,
                                               1);
    antenna->ForceDeploy();
    AntennaMessage current_status(antenna->GetStatus());
    TEST_ASSERT_EQUAL_UINT8(expected_final_state,
                            current_status.GetAntennaState());
}
