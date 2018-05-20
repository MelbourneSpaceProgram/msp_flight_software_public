#include <external/nanopb/pb_encode.h>
#include <external/sgp4/sgp4.h>
#include <src/adcs/state_estimators/location_estimator.h>
#include <src/config/unit_tests.h>
#include <src/messages/Tle.pb.h>
#include <src/payload_processor/commands/test_command.h>
#include <src/payload_processor/commands/tle_update_command.h>
#include <src/payload_processor/payload_processor.h>
#include <src/telecomms/lithium.h>
#include <test_runners/payload_processor_tests.h>
#include <test_runners/unity.h>
#include <ti/sysbios/BIOS.h>
#include <ti/sysbios/knl/Mailbox.h>

void TestPayloadProcessor(void) {
    byte payload[Lithium::kMaxReceivedSize] = {0};

    // Set the bytes necessary for 2 x test command
    payload[0] = 1;
    payload[1] = 0;
    payload[2] = TestCommand::kTestValue1;
    payload[3] = TestCommand::kTestValue2;
    payload[4] = 1;
    payload[5] = 0;
    payload[6] = TestCommand::kTestValue1;
    payload[7] = TestCommand::kTestValue2;

    PayloadProcessor payload_processor;
    TEST_ASSERT(payload_processor.ParseAndExecuteCommands(payload));
}

void TestForceResetCommand(void) {
    if (!force_reset_command_test_enabled) {
        TEST_IGNORE_MESSAGE("Force reset command test ignored");
    }

    byte buffer[4];
    buffer[0] = 4;  // 4 indicates a force reset command
    buffer[1] = 0;
    buffer[2] = 0;
    buffer[3] = 0;

    PayloadProcessor payload_processor;
    TEST_ASSERT(payload_processor.ParseAndExecuteCommands(buffer));
}

void TestTleUpdateCommand(void) {
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

    uint8_t buffer[Tle_size + 2];
    buffer[0] = 3;  // 3 indicates a tle update command
    buffer[1] = 0;
    pb_ostream_t stream =
        pb_ostream_from_buffer(buffer + 2, sizeof(buffer) - 2);
    pb_encode(&stream, Tle_fields, &test_tle);

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

    TEST_ASSERT(generated_satrec.no_kozai ==
                test_tle.mean_motion / Sgp4Utils::xpdotp);
    TEST_ASSERT(generated_satrec.inclo ==
                test_tle.inclination * Sgp4Utils::kDegreesToRadians);
    TEST_ASSERT(generated_satrec.nodeo ==
                test_tle.raan * Sgp4Utils::kDegreesToRadians);
    TEST_ASSERT(generated_satrec.argpo = test_tle.argument_of_perigee *
                                         Sgp4Utils::kDegreesToRadians);
    TEST_ASSERT(generated_satrec.mo ==
                test_tle.mean_anomaly * Sgp4Utils::kDegreesToRadians);
    TEST_ASSERT(generated_satrec.bstar == test_tle.bstar_drag);
}
