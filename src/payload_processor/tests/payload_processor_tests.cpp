#include <CppUTest/TestHarness.h>
#include <external/nanopb/pb_encode.h>
#include <external/sgp4/sgp4.h>
#include <src/adcs/state_estimators/location_estimator.h>
#include <src/board/i2c/io_expander/io_expander.h>
#include <src/config/satellite.h>
#include <src/config/unit_tests.h>
#include <src/database/circular_buffer_nanopb.h>
#include <src/database/sd_card.h>
#include <src/messages/CurrentReading.pb.h>
#include <src/messages/IoExpanderToggleCommandPayload.pb.h>
#include <src/messages/Time.pb.h>
#include <src/messages/Tle.pb.h>
#include <src/payload_processor/commands/io_expander_toggle_command.h>
#include <src/payload_processor/commands/lithium_beacon_period_command.h>
#include <src/payload_processor/commands/science_data_command.h>
#include <src/payload_processor/commands/test_command.h>
#include <src/payload_processor/commands/tle_update_command.h>
#include <src/payload_processor/payload_processor.h>
#include <src/payload_processor/runnable_payload_processor.h>
#include <src/sensors/measurable_id.h>
#include <src/sensors/measurable_manager.h>
#include <src/telecomms/lithium.h>
#include <src/telecomms/msp_payloads/test_ones_payload.h>
#include <src/telecomms/runnable_beacon.h>
#include <src/util/etl_utils.h>
#include <src/util/message_codes.h>
#include <src/util/nanopb_utils.h>
#include <src/util/satellite_time_source.h>
#include <src/util/task_utils.h>
#include <stdio.h>
#include <ti/sysbios/BIOS.h>

TEST_GROUP(PayloadProcessor){};

void BuildTwoTestCommands(byte payload_buffer[]) {
    // Build two test commands
    payload_buffer[0] = 1;
    payload_buffer[1] = 0;
    payload_buffer[2] = TestCommand::kTestValue1;
    payload_buffer[3] = TestCommand::kTestValue2;
    payload_buffer[4] = 1;
    payload_buffer[5] = 0;
    payload_buffer[6] = TestCommand::kTestValue1;
    payload_buffer[7] = TestCommand::kTestValue2;
    payload_buffer[8] = PayloadProcessor::GetEndTerminator();
    payload_buffer[9] = PayloadProcessor::GetEndTerminator();
}

TEST(PayloadProcessor, TestPayloadProcessor) {
    byte payload[Lithium::kMaxReceivedUplinkSize] = {0};

    // Set the bytes necessary for 2 x test command
    BuildTwoTestCommands(payload);

    PayloadProcessor payload_processor;
    CHECK(payload_processor.ParseAndExecuteCommands(payload));
}

TEST(PayloadProcessor, TestForceResetCommand) {
    if (!kForceResetCommandEnabled) {
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
    CHECK(test_payload_processor.ParseAndExecuteCommands(buffer));

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
    if (!kTcomBoardAvailable) {
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
    LithiumBeaconPeriodCommandPayload injected_beacon_period = {5000};
    NanopbEncode(LithiumBeaconPeriodCommandPayload)(
        buffer + PayloadProcessor::GetCommandCodeLength(),
        injected_beacon_period);

    // Set end terminators
    buffer[PayloadProcessor::GetCommandCodeLength() +
           LithiumBeaconPeriodCommandPayload_size] =
        PayloadProcessor::GetEndTerminator();
    buffer[PayloadProcessor::GetCommandCodeLength() +
           LithiumBeaconPeriodCommandPayload_size + 1] =
        PayloadProcessor::GetEndTerminator();

    PayloadProcessor payload_processor;
    CHECK(payload_processor.ParseAndExecuteCommands(buffer));
    CHECK_EQUAL(RunnableBeacon::GetBeaconPeriodMs(), 5000);

    // Reset to nominal
    injected_beacon_period = {kNominalBeaconPeriodMs};
    NanopbEncode(LithiumBeaconPeriodCommandPayload)(
        buffer + PayloadProcessor::GetCommandCodeLength(),
        injected_beacon_period);
    CHECK(payload_processor.ParseAndExecuteCommands(buffer));
    CHECK_EQUAL(RunnableBeacon::GetBeaconPeriodMs(), kNominalBeaconPeriodMs);
}

TEST(PayloadProcessor, TestScienceDataCommand) {
    try {
        if (!kSdCardAvailable) {
            TEST_EXIT
        }
        // Generate a fake science data command
        byte buffer[Lithium::kMaxReceivedUplinkSize] = {0};

        // Set command code - indicates a science data command
        buffer[0] = kScienceDataCommand;
        buffer[1] = 0;

        // Put requested ID in the buffer
        buffer[3] = kTestCurrent;
        buffer[4] = 0;

        // Put requested timestamp in the buffer
        Time requested_time = SatelliteTimeSource::GetTime();
        NanopbEncode(Time)(buffer + PayloadProcessor::GetCommandCodeLength() +
                               sizeof(uint16_t),
                           requested_time);

        // Create a CircularBuffer and put a value in it
        char filename[4];
        sprintf(filename, "%3d", kTestCurrent);
        CircularBufferNanopb(CurrentReading)::Create(filename, 10);

        CurrentReading test_current =
            MeasurableManager::GetInstance()
                ->ReadNanopbMeasurable<CurrentReading>(kComInI2, 0);
        CircularBufferNanopb(CurrentReading)::WriteMessage(filename,
                                                           test_current);

        PayloadProcessor payload_processor;
        CHECK(payload_processor.ParseAndExecuteCommands(buffer));

        SdCard::GetInstance()->FileDelete(filename);
    } catch (etl::exception& e) {
        EtlUtils::LogException(e);
        FAIL("Uncaught exception in test");
    }
}

// TODO(dingbenjamin): Figure out how to teardown for just this test
TEST(PayloadProcessor, TestIoExpanderToggleCommand) {
    byte buffer[Lithium::kMaxReceivedUplinkSize] = {0};

    // Set nanopb message
    // Toggle comms regulators expander
    IoExpanderToggleCommandPayload en_1_off = {
        static_cast<uint32_t>(IoExpander::kCommsIoExpander),
        static_cast<uint32_t>(IoExpander::kIoExpanderPinTelecomsEn1),
        static_cast<uint32_t>(IoExpanderToggleCommand::kToggleOff), 1000};
    IoExpanderToggleCommandPayload en_3_off = {
        static_cast<uint32_t>(IoExpander::kCommsIoExpander),
        static_cast<uint32_t>(IoExpander::kIoExpanderPinTelecomsEn3),
        static_cast<uint32_t>(IoExpanderToggleCommand::kToggleOff), 1000};
    IoExpanderToggleCommandPayload en_4_off = {
        static_cast<uint32_t>(IoExpander::kCommsIoExpander),
        static_cast<uint32_t>(IoExpander::kIoExpanderPinTelecomsEn4),
        static_cast<uint32_t>(IoExpanderToggleCommand::kToggleOff), 1000};
    IoExpanderToggleCommandPayload en_5_off = {
        static_cast<uint32_t>(IoExpander::kCommsIoExpander),
        static_cast<uint32_t>(IoExpander::kIoExpanderPinTelecomsEn5),
        static_cast<uint32_t>(IoExpanderToggleCommand::kToggleOff), 1000};

    // Set command code - 12 indicates an IO toggle command
    buffer[0] = 12;
    buffer[1] = 0;
    NanopbEncode(IoExpanderToggleCommandPayload)(
        buffer + PayloadProcessor::GetCommandCodeLength(), en_1_off);

    buffer[PayloadProcessor::GetCommandCodeLength() +
           IoExpanderToggleCommandPayload_size] = 12;
    NanopbEncode(IoExpanderToggleCommandPayload)(
        buffer + 2 * PayloadProcessor::GetCommandCodeLength() +
            IoExpanderToggleCommandPayload_size,
        en_3_off);

    buffer[(IoExpanderToggleCommandPayload_size +
            PayloadProcessor::GetCommandCodeLength()) *
           2] = 12;
    NanopbEncode(IoExpanderToggleCommandPayload)(
        buffer + 3 * PayloadProcessor::GetCommandCodeLength() +
            2 * IoExpanderToggleCommandPayload_size,
        en_4_off);

    buffer[(IoExpanderToggleCommandPayload_size +
            PayloadProcessor::GetCommandCodeLength()) *
           3] = 12;
    NanopbEncode(IoExpanderToggleCommandPayload)(
        buffer + 4 * PayloadProcessor::GetCommandCodeLength() +
            3 * IoExpanderToggleCommandPayload_size,
        en_5_off);

    // Set end terminators
    buffer[(PayloadProcessor::GetCommandCodeLength() +
            IoExpanderToggleCommandPayload_size) *
           4] = PayloadProcessor::GetEndTerminator();
    buffer[(PayloadProcessor::GetCommandCodeLength() +
            IoExpanderToggleCommandPayload_size) *
               4 +
           1] = PayloadProcessor::GetEndTerminator();

    PayloadProcessor payload_processor;
    CHECK(payload_processor.ParseAndExecuteCommands(buffer));

    // Check Lithium is now unable to transmit
    TaskUtils::SleepMilli(2000);
    TestOnesPayload ones;
    CHECK_FALSE(Lithium::GetInstance()->Transmit(&ones));

    // Turn back on
    IoExpanderToggleCommandPayload en_1_on = {
        static_cast<uint32_t>(IoExpander::kCommsIoExpander),
        static_cast<uint32_t>(IoExpander::kIoExpanderPinTelecomsEn1),
        static_cast<uint32_t>(IoExpanderToggleCommand::kToggleOn), 1000};
    IoExpanderToggleCommandPayload en_3_on = {
        static_cast<uint32_t>(IoExpander::kCommsIoExpander),
        static_cast<uint32_t>(IoExpander::kIoExpanderPinTelecomsEn3),
        static_cast<uint32_t>(IoExpanderToggleCommand::kToggleOn), 1000};
    IoExpanderToggleCommandPayload en_4_on = {
        static_cast<uint32_t>(IoExpander::kCommsIoExpander),
        static_cast<uint32_t>(IoExpander::kIoExpanderPinTelecomsEn4),
        static_cast<uint32_t>(IoExpanderToggleCommand::kToggleOn), 1000};
    IoExpanderToggleCommandPayload en_5_on = {
        static_cast<uint32_t>(IoExpander::kCommsIoExpander),
        static_cast<uint32_t>(IoExpander::kIoExpanderPinTelecomsEn5),
        static_cast<uint32_t>(IoExpanderToggleCommand::kToggleOn), 1000};

    NanopbEncode(IoExpanderToggleCommandPayload)(
        buffer + PayloadProcessor::GetCommandCodeLength(), en_1_on);

    buffer[PayloadProcessor::GetCommandCodeLength() +
           IoExpanderToggleCommandPayload_size] = 12;
    NanopbEncode(IoExpanderToggleCommandPayload)(
        buffer + 2 * PayloadProcessor::GetCommandCodeLength() +
            IoExpanderToggleCommandPayload_size,
        en_3_on);
    NanopbEncode(IoExpanderToggleCommandPayload)(
        buffer + 3 * PayloadProcessor::GetCommandCodeLength() +
            2 * IoExpanderToggleCommandPayload_size,
        en_4_on);
    NanopbEncode(IoExpanderToggleCommandPayload)(
        buffer + 4 * PayloadProcessor::GetCommandCodeLength() +
            3 * IoExpanderToggleCommandPayload_size,
        en_5_on);

    CHECK(payload_processor.ParseAndExecuteCommands(buffer));
    TaskUtils::SleepMilli(2000);
    CHECK(Lithium::GetInstance()->Transmit(&ones));
}

TEST(PayloadProcessor, TestSequence) {
    PayloadProcessor payload_processor;
    RunnablePayloadProcessor::check_sequence = true;
    RunnablePayloadProcessor::check_hmac = false;
    RunnablePayloadProcessor::use_fec = false;
    RunnablePayloadProcessor::sequence = 0;

    // Build a command that is reused in the test
    // Don't need to set HMAC or FEC as these are turned off for now
    byte rpp_payload[Lithium::kMaxReceivedUplinkSize] = {0};
    BuildTwoTestCommands(
        &rpp_payload[RunnablePayloadProcessor::kMspCommandIndex]);

    // Check start
    for (uint16_t i = 0; i <= 50; i++) {
        rpp_payload[RunnablePayloadProcessor::kMspSequenceNumberIndex] =
            static_cast<uint8_t>(i >> 8);
        rpp_payload[RunnablePayloadProcessor::kMspSequenceNumberIndex + 1] =
            static_cast<uint8_t>(i % 256);
        CHECK(RunnablePayloadProcessor::CheckSequence(rpp_payload));
        CHECK(payload_processor.ParseAndExecuteCommands(
            &rpp_payload[RunnablePayloadProcessor::kMspCommandIndex]));

        // Check invalid
        CHECK_FALSE(RunnablePayloadProcessor::CheckSequence(rpp_payload));
    }

    // Check end (skip the middle)
    RunnablePayloadProcessor::sequence = 65530;

    for (uint16_t i = RunnablePayloadProcessor::sequence; i < UINT16_MAX; i++) {
        rpp_payload[RunnablePayloadProcessor::kMspSequenceNumberIndex] =
            static_cast<uint8_t>(i >> 8);
        rpp_payload[RunnablePayloadProcessor::kMspSequenceNumberIndex + 1] =
            static_cast<uint8_t>(i % 256);
        CHECK(RunnablePayloadProcessor::CheckSequence(rpp_payload));
        CHECK(payload_processor.ParseAndExecuteCommands(
            &rpp_payload[RunnablePayloadProcessor::kMspCommandIndex]));

        // Check invalid
        CHECK_FALSE(RunnablePayloadProcessor::CheckSequence(rpp_payload));
    }

    // Check last value and wraparound

    rpp_payload[RunnablePayloadProcessor::kMspSequenceNumberIndex] =
        static_cast<uint8_t>(UINT16_MAX >> 8);
    rpp_payload[RunnablePayloadProcessor::kMspSequenceNumberIndex + 1] =
        static_cast<uint8_t>(UINT16_MAX % 256);
    CHECK(RunnablePayloadProcessor::CheckSequence(rpp_payload));
    CHECK(payload_processor.ParseAndExecuteCommands(
        &rpp_payload[RunnablePayloadProcessor::kMspCommandIndex]));

    rpp_payload[RunnablePayloadProcessor::kMspSequenceNumberIndex] = 0;
    rpp_payload[RunnablePayloadProcessor::kMspSequenceNumberIndex + 1] = 0;
    CHECK(RunnablePayloadProcessor::CheckSequence(rpp_payload));
    CHECK(payload_processor.ParseAndExecuteCommands(
        &rpp_payload[RunnablePayloadProcessor::kMspCommandIndex]));

    // TODO(dingbenjamin): Move this to teardown
    RunnablePayloadProcessor::check_sequence = kCheckSequenceDefault;
    RunnablePayloadProcessor::check_hmac = kCheckHmacDefault;
    RunnablePayloadProcessor::use_fec = kUseFecDefault;
    RunnablePayloadProcessor::sequence = 0;
}
