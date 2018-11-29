#ifndef SRC_TELECOMMS_RUNNABLE_CONTINUOUS_TRANSMIT_SHUTOFF_H_
#define SRC_TELECOMMS_RUNNABLE_CONTINUOUS_TRANSMIT_SHUTOFF_H_

#include <src/tasks/runnable.h>
#include <src/telecomms/lithium.h>
#include "../util/tirtos_utils.h"

class RunnableContinuousTransmitShutoff : public Runnable {
   public:
    fnptr GetRunnablePointer();
    RunnableContinuousTransmitShutoff();
    static constexpr uint16_t kTotalSeconds = 600;
    static constexpr uint8_t kBucketSeconds = 10;
    static constexpr uint16_t kMillisecondsInSecond = 1000;
    static constexpr uint16_t kMaxNumberTransmissions = 300;
    static constexpr uint16_t kNumBuckets = kTotalSeconds / kBucketSeconds;
    static void IncrementBucketCount();
    static uint16_t GetBucketCount();
    static bool TransmissionRateUnderThreshold();
    static void ClearBuckets();

   private:
    static void StartCounter();
    static uint16_t bucket_count;
    static uint16_t transmission_buckets[kNumBuckets];
};

#endif  // SRC_TELECOMMS_RUNNABLE_CONTINUOUS_TRANSMIT_SHUTOFF_H_
