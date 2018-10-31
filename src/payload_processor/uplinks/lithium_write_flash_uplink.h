#ifndef SRC_PAYLOAD_PROCESSOR_UPLINKS_LITHIUM_WRITE_FLASH_UPLINK_H_
#define SRC_PAYLOAD_PROCESSOR_UPLINKS_LITHIUM_WRITE_FLASH_UPLINK_H_

#include <src/payload_processor/uplinks/uplink.h>
#include <src/telecomms/lithium_md5.h>

class LithiumWriteFlashUplink : public Uplink {
   public:
    explicit LithiumWriteFlashUplink(byte* payload);
    bool ExecuteUplink();

   private:
    static constexpr uint8_t kLithiumWriteFlashUplinkArgumentLength =
        LithiumMd5Payload_size;
    LithiumMd5 md5;
};

#endif  // SRC_PAYLOAD_PROCESSOR_SRC_PAYLOAD_PROCESSOR_UPLINKS_LITHIUM_WRITE_FLASH_UPLINK_H_
