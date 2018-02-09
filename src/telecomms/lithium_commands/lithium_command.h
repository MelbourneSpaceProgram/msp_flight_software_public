#ifndef SRC_TELECOMMS_LITHIUM_COMMANDS_LITHIUM_COMMAND_H_
#define SRC_TELECOMMS_LITHIUM_COMMANDS_LITHIUM_COMMAND_H_

#include <src/messages/message.h>
#include <src/messages/serialised_message_builder.h>
#include <src/util/data_types.h>

class LithiumCommand : Message {
   public:
    virtual ~LithiumCommand() {}
    virtual uint16_t GetLithiumPayloadSize() const = 0;
    virtual const byte &GetCommandCode() const = 0;
    virtual uint16_t GetReplyPayloadSize() const = 0;
    SerialisedMessage SerialiseTo(byte *serial_buffer);
    uint16_t GetSerialisedSize() const;

    static const uint16_t kNoPayload = 0;

   protected:
    LithiumCommand(byte command_code, Message *lithium_payload);

   private:
    // Documentation on building a Lithium header, tail, and checksum can be
    // found in the the Radio Interface Manual at CS1-TXT-14
    void BuildHeader(SerialisedMessageBuilder *builder);
    void BuildTail(SerialisedMessageBuilder *builder);

    Message *lithium_payload;
    byte command_code;
};

#endif  // SRC_TELECOMMS_LITHIUM_COMMANDS_LITHIUM_COMMAND_H_
