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

    void *GetReplyBuffer() { return reply_buffer; }

    LithiumResponseType GetResponse() {
        // TODO(dingbenjamin): Check if the cast succeeds
        LithiumResponseType *response =
            reinterpret_cast<LithiumResponseType *>(reply_buffer);
        return *response;
    }

   private:
    byte reply_buffer[Lithium::kLithiumHeaderSize];
};

#endif  // SRC_TELECOMMS_LITHIUM_COMMANDS_LITHIUM_RESPONSE_COMMAND_H_
