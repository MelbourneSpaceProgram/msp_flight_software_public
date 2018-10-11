#include <src/telecomms/lithium.h>
#include <CppUTest/TestHarness.h>
#include <src/config/unit_tests.h>
#include <src/telecomms/lithium.h>
#include <src/telecomms/msp_payloads/test_payload.h>
#include <src/telecomms/runnable_continuous_transmit_shutoff.h>
#include <src/telecomms/lithium_commands/transmit_command.h>

TEST_GROUP(LithiumTransmit) {
    void setup() {
        RunnableContinuousTransmitShutoff::ClearBuckets();
    };

    void teardown() {
    };
};

TEST(LithiumTransmit, TestLithiumShutoff) {
  int num_transmits = 400;

  CHECK_EQUAL(RunnableContinuousTransmitShutoff::GetBucketCount(), 0);

  TestPayload test_payload;
  for(int i=0; i<num_transmits; i++){
    TransmitCommand dummy_command(&test_payload);
  }
  CHECK_EQUAL(RunnableContinuousTransmitShutoff::GetBucketCount(), num_transmits);

  TaskUtils::SleepMilli(RunnableContinuousTransmitShutoff::kBucketSeconds*RunnableContinuousTransmitShutoff::kMillisecondsInSecond + 100);

  CHECK(!RunnableContinuousTransmitShutoff::TransmissionRateUnderThreshold());
  CHECK(!Lithium::GetInstance()->IsTransmitEnabled());
}

TEST(LithiumTransmit, TestLithiumMultipleBuckets) {
  int num_transmits = 300;
  int num_buckets = 3;

  CHECK_EQUAL(RunnableContinuousTransmitShutoff::GetBucketCount(), 0);

  for(int i=0; i<num_buckets; i++){
      TestPayload test_payload;
    for(int i=0; i<num_transmits/num_buckets; i++){
        TransmitCommand dummy_command(&test_payload);
    }
    CHECK_EQUAL(RunnableContinuousTransmitShutoff::GetBucketCount(), num_transmits/num_buckets);
    TaskUtils::SleepMilli(RunnableContinuousTransmitShutoff::kBucketSeconds*RunnableContinuousTransmitShutoff::kMillisecondsInSecond + 100);
  }

  CHECK(!RunnableContinuousTransmitShutoff::TransmissionRateUnderThreshold());
  CHECK(!Lithium::GetInstance()->IsTransmitEnabled());
}

TEST(LithiumTransmit, TestLithiumUnderThreshold) {
  int num_transmits = 100;

  CHECK_EQUAL(RunnableContinuousTransmitShutoff::GetBucketCount(), 0);
  TestPayload test_payload;
  for(int i=0; i<num_transmits; i++){
      TransmitCommand dummy_command(&test_payload);
  }

  CHECK_EQUAL(RunnableContinuousTransmitShutoff::GetBucketCount(), num_transmits);

  CHECK(RunnableContinuousTransmitShutoff::TransmissionRateUnderThreshold());
  CHECK(Lithium::GetInstance()->IsTransmitEnabled());
}
