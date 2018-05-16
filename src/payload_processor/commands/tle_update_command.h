#ifndef SRC_PAYLOAD_PROCESSOR_COMMANDS_TLE_UPDATE_COMMAND_H_
#define SRC_PAYLOAD_PROCESSOR_COMMANDS_TLE_UPDATE_COMMAND_H_

#include <src/messages/Tle.pb.h>
#include <src/payload_processor/commands/command.h>
#include <ti/sysbios/BIOS.h>
#include <ti/sysbios/knl/Mailbox.h>

class TleUpdateCommand : public Command {
   public:
    TleUpdateCommand(byte* payload, byte command_argument_start_index);
    bool ExecuteCommand();
    static void SetTleUpdateCommandMailboxHandle(
        Mailbox_Handle tle_update_command_mailbox_handle);

   private:
    static const uint8_t kTleUpdateCommandArgumentLength = Tle_size;
    Tle new_tle;
    static Mailbox_Handle tle_update_command_mailbox_handle;
};

#endif  // SRC_PAYLOAD_PROCESSOR_COMMANDS_TLE_UPDATE_COMMAND_H_
