#ifndef SRC_TELECOMMS_LITHIUM_COMMANDS_LITHIUM_COMMAND_H_
#define SRC_TELECOMMS_LITHIUM_COMMANDS_LITHIUM_COMMAND_H_

#include <src/messages/message.h>
#include <src/messages/serialised_message_builder.h>
#include <src/util/data_types.h>

class LithiumCommand : Message {
   public:
    virtual ~LithiumCommand() {}
    SerialisedMessage SerialiseTo(byte *serial_buffer);
    uint16_t GetSerialisedSize() const;
    virtual uint16_t GetLithiumPayloadSize() const = 0;

   protected:
    LithiumCommand(byte command_code, Message *lithium_payload);

   private:
    static const uint8_t kLithiumCommandHeaderSize = 8;
    static const uint8_t kLithiumCommandTailSize = 2;
    static const byte kSyncCharOne = 0x48;
    static const byte kSyncCharTwo = 0x65;
    static const byte kDirectionIntoLithium = 0x10;
    static const byte kDirectionOutOfLithium = 0x20;

    // Documentation on building a Lithium header, tail, and checksum can be
    // found in the the Radio Interface Manual at CS1-TXT-14
    static void CalcChecksum(byte *checksum, byte *data, uint16_t data_size);
    void BuildHeader(SerialisedMessageBuilder *builder);
    void BuildTail(SerialisedMessageBuilder *builder);

    Message *lithium_payload;
    byte command_code;
};

#endif  // SRC_TELECOMMS_LITHIUM_COMMANDS_LITHIUM_COMMAND_H_
