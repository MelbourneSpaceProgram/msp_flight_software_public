#ifndef SRC_PAYLOAD_PROCESSOR_UPLINKS_CLEAR_EXCEPTIONS_UPLINK_H_
#define SRC_PAYLOAD_PROCESSOR_UPLINKS_CLEAR_EXCEPTIONS_UPLINK_H_

#include <src/payload_processor/uplinks/uplink.h>

class ClearExceptionsUplink : public Uplink {
   public:
    explicit ClearExceptionsUplink(byte* payload);
    bool ExecuteUplink();

   private:
	// TODO(dingbenjamin): Make a Nanopb for this
	const uint8_t error_id;
    static constexpr uint8_t kClearExceptionsUplinkArgumentLength = sizeof(uint8_t);
    static constexpr uint8_t kClearAll = 255;
};

#endif  // SRC_PAYLOAD_PROCESSOR_SRC_PAYLOAD_PROCESSOR_UPLINKS_CLEAR_EXCEPTIONS_UPLINK_H_
