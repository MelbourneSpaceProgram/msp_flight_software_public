#ifndef SRC_PAYLOAD_PROCESSOR_COMMANDS_TLE_UPDATE_UPLINK_H_
#define SRC_PAYLOAD_PROCESSOR_COMMANDS_TLE_UPDATE_UPLINK_H_

#include <src/messages/Tle.pb.h>
#include <ti/sysbios/knl/Mailbox.h>
#include <src/payload_processor/uplinks/uplink.h>

class TleUpdateUplink : public Uplink {
   public:
    explicit TleUpdateUplink(byte* payload);
    bool ExecuteUplink();
    static void SetTleUpdateUplinkMailboxHandle(
        Mailbox_Handle tle_update_command_mailbox_handle);

   private:
    Tle new_tle;
    static Mailbox_Handle tle_update_command_mailbox_handle;
};

#endif  // SRC_PAYLOAD_PROCESSOR_COMMANDS_TLE_UPDATE_UPLINK_H_
