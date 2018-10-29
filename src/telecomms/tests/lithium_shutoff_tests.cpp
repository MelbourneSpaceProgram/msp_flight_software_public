#include <CppUTest/TestHarness.h>
#include <src/config/unit_tests.h>
#include <src/telecomms/lithium.h>
#include <src/telecomms/lithium_commands/transmit_command.h>
#include <src/telecomms/msp_payloads/test_payload.h>
#include <src/telecomms/runnable_continuous_transmit_shutoff.h>

TEST_GROUP(LithiumShutoff) {
    void setup() {
        Lithium::GetInstance()->SetTransmitEnabled(true);
        Lithium::GetInstance()->UnlockState(Lithium::kCriticalTempCondition);
        Lithium::GetInstance()->UnlockState(
            Lithium::kContinuousTransmitCondition);
        RunnableContinuousTransmitShutoff::ClearBuckets();
    };
    void teardown() {
        Lithium::GetInstance()->SetTransmitEnabled(true);
        Lithium::GetInstance()->UnlockState(Lithium::kCriticalTempCondition);
        Lithium::GetInstance()->UnlockState(
            Lithium::kContinuousTransmitCondition);
    };
};

TEST(LithiumShutoff, TestLithiumShutoff) {
    uint16_t num_transmits = 400;

    CHECK_EQUAL(RunnableContinuousTransmitShutoff::GetBucketCount(), 0);

    TestPayload test_payload;
    for (uint16_t i = 0; i < num_transmits; i++) {
        TransmitCommand dummy_command(&test_payload);
    }
    CHECK_EQUAL(RunnableContinuousTransmitShutoff::GetBucketCount(),
                num_transmits);

    TaskUtils::SleepMilli(
        RunnableContinuousTransmitShutoff::kBucketSeconds *
            RunnableContinuousTransmitShutoff::kMillisecondsInSecond +
        100);

    CHECK(!RunnableContinuousTransmitShutoff::TransmissionRateUnderThreshold());
    CHECK(!Lithium::GetInstance()->IsTransmitEnabled());
}

TEST(LithiumShutoff, TestLithiumMultipleBuckets) {
    uint16_t num_transmits = 300;
    uint16_t num_buckets = 3;

    CHECK_EQUAL(RunnableContinuousTransmitShutoff::GetBucketCount(), 0);

    for (uint16_t i = 0; i < num_buckets; i++) {
        TestPayload test_payload;
        for (uint16_t i = 0; i < num_transmits / num_buckets; i++) {
            TransmitCommand dummy_command(&test_payload);
        }
        CHECK_EQUAL(RunnableContinuousTransmitShutoff::GetBucketCount(),
                    num_transmits / num_buckets);
        TaskUtils::SleepMilli(
            RunnableContinuousTransmitShutoff::kBucketSeconds *
                RunnableContinuousTransmitShutoff::kMillisecondsInSecond +
            100);
    }

    CHECK(!RunnableContinuousTransmitShutoff::TransmissionRateUnderThreshold());
    CHECK(!Lithium::GetInstance()->IsTransmitEnabled());
}

TEST(LithiumShutoff, TestLithiumUnderThreshold) {
    uint16_t num_transmits = 100;

    CHECK_EQUAL(RunnableContinuousTransmitShutoff::GetBucketCount(), 0);
    TestPayload test_payload;
    for (uint16_t i = 0; i < num_transmits; i++) {
        TransmitCommand dummy_command(&test_payload);
    }

    CHECK_EQUAL(RunnableContinuousTransmitShutoff::GetBucketCount(),
                num_transmits);

    CHECK(RunnableContinuousTransmitShutoff::TransmissionRateUnderThreshold());
    CHECK(Lithium::GetInstance()->IsTransmitEnabled());
}
