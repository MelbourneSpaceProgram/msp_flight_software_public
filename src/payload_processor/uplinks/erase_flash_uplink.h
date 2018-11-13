#ifndef SRC_MESSAGES_UPLINKS_ERASE_FLASH_UPLINK_H_
#define SRC_MESSAGES_UPLINKS_ERASE_FLASH_UPLINK_H_

#include <src/payload_processor/uplinks/uplink.h>
#include <src/messages/EraseFlashUplinkPayload.pb.h>

class EraseFlashUplink : public Uplink {
public:
    explicit EraseFlashUplink(byte* payload);
    bool ExecuteUplink();
private:
    EraseFlashUplinkPayload erase_flash;
};

#endif  // SRC_MESSAGES_UPLINKS_ERASE_FLASH_UPLINK_H_
