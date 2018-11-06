#ifndef SRC_PAYLOAD_PROCESSOR_UPLINKS_QUERY_NUM_EXCEPTIONS_UPLINK_H_
#define SRC_PAYLOAD_PROCESSOR_UPLINKS_QUERY_NUM_EXCEPTIONS_UPLINK_H_

#include <src/payload_processor/uplinks/uplink.h>

class QueryNumExceptionsUplink : public Uplink {
   public:
    explicit QueryNumExceptionsUplink(byte* payload);
    bool ExecuteUplink();

   private:
    static constexpr uint8_t kQueryNumExceptionsUplinkArgumentLength = 0;
};

#endif  // SRC_PAYLOAD_PROCESSOR_SRC_PAYLOAD_PROCESSOR_UPLINKS_QUERY_NUM_EXCEPTIONS_UPLINK_H_
