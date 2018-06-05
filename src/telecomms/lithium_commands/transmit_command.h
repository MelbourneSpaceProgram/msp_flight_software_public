#ifndef SRC_TELECOMMS_LITHIUM_COMMANDS_TRANSMIT_COMMAND_H_
#define SRC_TELECOMMS_LITHIUM_COMMANDS_TRANSMIT_COMMAND_H_

#include <src/telecomms/lithium_commands/lithium_command.h>
#include <src/telecomms/lithium_commands/transmit_command.h>
#include <src/telecomms/msp_payloads/transmit_packet.h>

class TransmitCommand : public LithiumCommand {
   public:
    TransmitCommand(TransmitPayload *payload, uint8_t tx_count,
                    uint8_t total_rx_count, uint8_t valid_rx_count);
    virtual ~TransmitCommand() {}
    uint16_t GetLithiumPayloadSize() const;
    uint16_t GetReplyPayloadSize() const;
    const byte &GetCommandCode() const;

   private:
    static const byte kTransmitCommandCode = 0x03;
    TransmitPacket transmit_packet;
};

#endif  // SRC_TELECOMMS_LITHIUM_COMMANDS_TRANSMIT_COMMAND_H_
