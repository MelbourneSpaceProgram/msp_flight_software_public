#ifndef SRC_PAYLOAD_PROCESSOR_UPLINKS_QUERY_EXCEPTIONS_UPLINK_H_
#define SRC_PAYLOAD_PROCESSOR_UPLINKS_QUERY_EXCEPTIONS_UPLINK_H_

#include <src/payload_processor/uplinks/uplink.h>

class QueryExceptionsUplink : public Uplink {
   public:
    explicit QueryExceptionsUplink(byte* payload);
    bool ExecuteUplink();

   private:
    // TODO(dingbenjamin): Make a Nanopb for this
    const uint8_t error_id;
    static constexpr uint8_t kQueryExceptionsUplinkArgumentLength =
        sizeof(uint8_t);
};

#endif  // SRC_PAYLOAD_PROCESSOR_SRC_PAYLOAD_PROCESSOR_UPLINKS_QUERY_EXCEPTIONS_UPLINK_H_
