#include <src/tasks/runnable.h>
#include <src/telecomms/lithium_commands/transmit_command.h>
#include <src/telecomms/lithium_utils.h>
#include <src/telecomms/runnable_continuous_transmit_shutoff.h>
#include <src/util/satellite_time_source.h>
#include <src/util/task_utils.h>
#include <xdc/runtime/Log.h>

uint16_t RunnableContinuousTransmitShutoff::bucket_count = 0;
uint16_t RunnableContinuousTransmitShutoff::transmission_buckets[kNumBuckets] =
    {0};

fnptr RunnableContinuousTransmitShutoff::GetRunnablePointer() {
    return &RunnableContinuousTransmitShutoff::StartCounter;
}

RunnableContinuousTransmitShutoff::RunnableContinuousTransmitShutoff() {
    assert(kNumBuckets * kBucketSeconds == kTotalSeconds);
}

void RunnableContinuousTransmitShutoff::StartCounter() {
    int rolling_index = 0;
    while (1) {
        TaskUtils::SleepMilli(kBucketSeconds * kMillisecondsInSecond);

        transmission_buckets[rolling_index] = bucket_count;

        if (!TransmissionRateUnderThreshold()) {
            Lithium::GetInstance()->SetTransmitEnabled(false);
            Log_info0(
                "Continuous Transmission Detected: Shutting off Lithium "
                "Transmission");
        }

        bucket_count = 0;
        rolling_index = (rolling_index + 1) % kNumBuckets;
    }
}

bool RunnableContinuousTransmitShutoff::TransmissionRateUnderThreshold() {
    uint16_t lithium_count = 0;
    for (int i = 0; i < kNumBuckets; i++) {
        lithium_count += transmission_buckets[i];
    }
    return (lithium_count < kMaxNumberTransmissions);
}

void RunnableContinuousTransmitShutoff::IncrementBucketCount() {
    bucket_count++;
}

uint16_t RunnableContinuousTransmitShutoff::GetBucketCount() {
    return bucket_count;
}

void RunnableContinuousTransmitShutoff::ClearBuckets() {
    for (int i = 0; i < kNumBuckets; i++) {
        transmission_buckets[i] = 0;
    }
    bucket_count = 0;
}
