#ifndef SRC_COMMANDS_FORMAT_SD_COMMAND_H_
#define SRC_COMMANDS_FORMAT_SD_COMMAND_H_

#include <src/messages/FormatSdCommandPayload.pb.h>
#include <src/payload_processor/commands/command.h>

class FormatSdCommand : public Command {
   public:
    FormatSdCommand(byte* payload);
    bool ExecuteCommand();

   private:
    FormatSdCommandPayload format_sd_payload;
    uint32_t sd_index;
};

#endif  // SRC_COMMANDS_FORMAT_SD_COMMAND_H_
