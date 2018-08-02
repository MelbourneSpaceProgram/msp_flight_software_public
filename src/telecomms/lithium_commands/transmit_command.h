#ifndef SRC_TELECOMMS_LITHIUM_COMMANDS_TRANSMIT_COMMAND_H_
#define SRC_TELECOMMS_LITHIUM_COMMANDS_TRANSMIT_COMMAND_H_

#include <src/telecomms/lithium_commands/lithium_command.h>
#include <src/telecomms/lithium_commands/transmit_command.h>
#include <src/telecomms/msp_payloads/transmit_packet.h>

class TransmitCommand : public LithiumCommand {
   public:
    explicit TransmitCommand(TransmitPayload *payload);
    virtual ~TransmitCommand() {}
    uint16_t GetLithiumPayloadSize() const;
    uint16_t GetReplyPayloadSize() const;
    const byte &GetCommandCode() const;

   private:
    TransmitPacket transmit_packet;
};

#endif  // SRC_TELECOMMS_LITHIUM_COMMANDS_TRANSMIT_COMMAND_H_
