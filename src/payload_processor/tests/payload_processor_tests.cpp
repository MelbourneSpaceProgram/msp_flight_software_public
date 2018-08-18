#include <CppUTest/TestHarness.h>
#include <external/nanopb/pb_encode.h>
#include <external/sgp4/sgp4.h>
#include <src/adcs/state_estimators/location_estimator.h>
#include <src/config/satellite.h>
#include <src/config/unit_tests.h>
#include <src/messages/Tle.pb.h>
#include <src/payload_processor/commands/lithium_beacon_period_command.h>
#include <src/payload_processor/commands/test_command.h>
#include <src/payload_processor/commands/tle_update_command.h>
#include <src/payload_processor/payload_processor.h>
#include <src/telecomms/lithium.h>
#include <src/telecomms/runnable_beacon.h>
#include <src/util/message_codes.h>
#include <src/util/nanopb_utils.h>
#include <ti/sysbios/BIOS.h>
#include <ti/sysbios/knl/Mailbox.h>

TEST_GROUP(PayloadProcessor){};

TEST(PayloadProcessor, TestPayloadProcessor) {
    byte payload[Lithium::kMaxReceivedUplinkSize] = {0};

    // Set the bytes necessary for 2 x test command
    payload[0] = 1;
    payload[1] = 0;
    payload[2] = TestCommand::kTestValue1;
    payload[3] = TestCommand::kTestValue2;
    payload[4] = 1;
    payload[5] = 0;
    payload[6] = TestCommand::kTestValue1;
    payload[7] = TestCommand::kTestValue2;
    payload[8] = PayloadProcessor::GetEndTerminator();
    payload[9] = PayloadProcessor::GetEndTerminator();

    PayloadProcessor payload_processor;
    CHECK(payload_processor.ParseAndExecuteCommands(payload));
}

TEST(PayloadProcessor, TestForceResetCommand) {
    if (!force_reset_command_test_enabled) {
        TEST_EXIT
    }

    byte payload[Lithium::kMaxReceivedUplinkSize] = {0};

    payload[0] = 4;
    payload[1] = 0;  // 0x04 indicates a force reset command
    payload[2] = PayloadProcessor::GetEndTerminator();
    payload[3] = PayloadProcessor::GetEndTerminator();

    PayloadProcessor payload_processor;
    CHECK(payload_processor.ParseAndExecuteCommands(payload));
    FAIL("Software reset failed.");  // reset has failed if program
                                     // reaches this line
}

TEST(PayloadProcessor, TestTleUpdateCommand) {
    Tle test_tle;
    // TLE test data (from sgp4_tests.cpp)
    test_tle.epoch = 00179.78495062;
    test_tle.mean_motion = 10.824191574;
    test_tle.eccentricity_1e7 = 1859667;
    test_tle.inclination = 34.2682;
    test_tle.raan = 348.7242;
    test_tle.argument_of_perigee = 331.7664;
    test_tle.mean_anomaly = 19.3264;
    test_tle.bstar_drag = 0.000028098;

    uint8_t buffer[Lithium::kMaxReceivedUplinkSize] = {0};
    buffer[0] = 3;  // 3 indicates tle update command
    buffer[1] = 0;
    buffer[PayloadProcessor::GetCommandCodeLength() + Tle_size] =
        PayloadProcessor::GetEndTerminator();
    buffer[PayloadProcessor::GetCommandCodeLength() + Tle_size + 1] =
        PayloadProcessor::GetEndTerminator();
    NanopbEncode(Tle)(buffer + PayloadProcessor::GetCommandCodeLength(),
                      test_tle);

    // Create test mailbox
    Mailbox_Params_init(&LocationEstimator::tle_update_command_mailbox_params);
    Mailbox_Handle tle_update_command_mailbox_handle = Mailbox_create(
        sizeof(Tle), 1, &LocationEstimator::tle_update_command_mailbox_params,
        NULL);
    if (tle_update_command_mailbox_handle == NULL) {
        etl::exception e("Unable to create TLE update command mailbox",
                         __FILE__, __LINE__);
        throw e;
    }
    TleUpdateCommand::SetTleUpdateCommandMailboxHandle(
        tle_update_command_mailbox_handle);
    LocationEstimator::SetTleUpdateCommandMailboxHandle(
        tle_update_command_mailbox_handle);

    // Send the encoded TLE to a payload processor (which decodes it and posts
    // it to the TLE update mailbox)
    PayloadProcessor test_payload_processor;
    bool command_executed =
        test_payload_processor.ParseAndExecuteCommands(buffer);

    // Have a location estimator retrieve the decoded TLE from the mailbox and
    // update its internal satrec values
    LocationEstimator test_location_estimator;
    test_location_estimator.CheckForUpdatedTle();
    elsetrec generated_satrec = test_location_estimator.GetSatrec();

    CHECK(generated_satrec.no_kozai ==
          test_tle.mean_motion / Sgp4Utils::xpdotp);
    CHECK(generated_satrec.inclo ==
          test_tle.inclination * Sgp4Utils::kDegreesToRadians);
    CHECK(generated_satrec.nodeo ==
          test_tle.raan * Sgp4Utils::kDegreesToRadians);
    CHECK(generated_satrec.argpo ==
          test_tle.argument_of_perigee * Sgp4Utils::kDegreesToRadians);
    CHECK(generated_satrec.mo ==
          test_tle.mean_anomaly * Sgp4Utils::kDegreesToRadians);
    CHECK(generated_satrec.bstar == test_tle.bstar_drag);
}

TEST(PayloadProcessor, TestLithiumTestCommand) {
    if (!tcom_board_available) {
        TEST_EXIT
    }

    byte payload[Lithium::kMaxReceivedUplinkSize] = {0};

    payload[0] = static_cast<uint8_t>(kLithiumTestCommand);
    payload[1] = 0;  // 0x08 indicates a Lithium Test command
    payload[2] = PayloadProcessor::GetEndTerminator();
    payload[3] = PayloadProcessor::GetEndTerminator();

    PayloadProcessor payload_processor;
    CHECK(payload_processor.ParseAndExecuteCommands(payload));
    // Should also verify on the ground that 10 packets have been received
}

TEST(PayloadProcessor, TestLithiumBeaconPeriodCommand) {
    // Generate a fake lithium beacon period command
    byte buffer[Lithium::kMaxReceivedUplinkSize] = {0};

    // Set command code - 5 indicates a beacon period command
    buffer[0] = 5;
    buffer[1] = 0;

    // Set nanopb message
    LithiumBeaconPeriod injected_beacon_period = {5000};
    NanopbEncode(LithiumBeaconPeriod)(
        buffer + PayloadProcessor::GetCommandCodeLength(),
        injected_beacon_period);

    // Set end terminators
    buffer[PayloadProcessor::GetCommandCodeLength() +
           LithiumBeaconPeriod_size] = PayloadProcessor::GetEndTerminator();
    buffer[PayloadProcessor::GetCommandCodeLength() + LithiumBeaconPeriod_size +
           1] = PayloadProcessor::GetEndTerminator();

    PayloadProcessor payload_processor;
    CHECK(payload_processor.ParseAndExecuteCommands(buffer));
    CHECK_EQUAL(RunnableBeacon::GetBeaconPeriodMs(), 5000);

    // Reset to nominal
    injected_beacon_period = {kNominalBeaconPeriodMs};
    NanopbEncode(LithiumBeaconPeriod)(
        buffer + PayloadProcessor::GetCommandCodeLength(),
        injected_beacon_period);
    CHECK(payload_processor.ParseAndExecuteCommands(buffer));
    CHECK_EQUAL(RunnableBeacon::GetBeaconPeriodMs(), kNominalBeaconPeriodMs);
}
