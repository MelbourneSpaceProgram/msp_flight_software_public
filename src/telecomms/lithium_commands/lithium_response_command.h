#ifndef SRC_TELECOMMS_LITHIUM_COMMANDS_LITHIUM_RESPONSE_COMMAND_H_
#define SRC_TELECOMMS_LITHIUM_COMMANDS_LITHIUM_RESPONSE_COMMAND_H_

#include <src/telecomms/lithium.h>
#include <src/telecomms/lithium_commands/lithium_command.h>
#include <src/util/data_types.h>

class Message;

template <typename LithiumResponseType>
class LithiumResponseCommand : public LithiumCommand {
   public:
    LithiumResponseCommand(byte command_code, Message *lithium_payload)
        : LithiumCommand(command_code, lithium_payload) {}
    virtual ~LithiumResponseCommand(){};

    byte *GetReplyBuffer() { return reply_buffer; }
    LithiumResponseType GetParsedResponse();

   protected:
    byte reply_buffer[Lithium::kMaxReceivedLithiumResponseSize];
};

#endif  // SRC_TELECOMMS_LITHIUM_COMMANDS_LITHIUM_RESPONSE_COMMAND_H_
