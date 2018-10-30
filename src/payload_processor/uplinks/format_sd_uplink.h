#ifndef SRC_COMMANDS_FORMAT_SD_UPLINK_H_
#define SRC_COMMANDS_FORMAT_SD_UPLINK_H_

#include <src/messages/FormatSdUplinkPayload.pb.h>
#include <src/payload_processor/uplinks/uplink.h>

class FormatSdUplink : public Uplink {
   public:
    FormatSdUplink(byte* payload);
    bool ExecuteUplink();

   private:
    FormatSdUplinkPayload format_sd_payload;
    uint32_t sd_index;
};

#endif  // SRC_COMMANDS_FORMAT_SD_UPLINK_H_
