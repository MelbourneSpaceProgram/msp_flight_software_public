#ifndef SRC_TELECOMMS_LITHIUM_H_
#define SRC_TELECOMMS_LITHIUM_H_

#include <src/board/uart/uart.h>
#include <src/telecomms/lithium_commands/get_configuration_command.h>
#include <src/telecomms/lithium_commands/lithium_command.h>
#include <src/telecomms/lithium_commands/no_op_command.h>
#include <src/telecomms/lithium_commands/reset_system_command.h>
#include <src/telecomms/lithium_commands/transmit_command.h>
#include <src/telecomms/lithium_configuration.h>
#include <ti/sysbios/knl/Mailbox.h>

class LithiumCommand;
class LithiumEnableCommand;
class RunnableLithiumListener;
class PayloadProcessor;

class Lithium {
    friend class PayloadProcessor;         // For incrementing command success
    friend class RunnableLithiumListener;  // For incrementing received count
    friend class LithiumEnableCommand;

   public:
    static const uint16_t kMaxCommandSize = 300;
    static const uint16_t kMaxReceivedSize = 300;
    static const uint16_t kReceivedPacketSize = 265;

    static const uint8_t kLithiumHeaderSize = 8;
    static const uint8_t kLithiumTailSize = 2;
    static const uint8_t kLithiumSyncSize = 2;

    static const byte kSyncCharOne = 0x48;
    static const byte kSyncCharTwo = 0x65;
    static const byte kDirectionIntoLithium = 0x10;
    static const byte kDirectionOutOfLithium = 0x20;

    static const uint16_t kInterCommandTimeMilli = 250;

    static Lithium* GetInstance();  // Initial call is not thread safe
    static uint8_t GetTxCounter();
    static uint8_t GetRxCounter();
    static uint8_t GetCommandSuccessCounter();
    Mailbox_Handle GetMessageMailbox() const;
    Mailbox_Handle GetCommandResponseMailbox() const;
    const LithiumConfiguration& GetLithiumConfig() const;
    void SetLithiumConfig(const LithiumConfiguration& lithium_config);
    bool IsTransmitEnabled();
    bool DoCommand(LithiumCommand* command) const;

   private:
    static Lithium* instance;
    static const uint32_t kUartWriteTimeoutMilli = 500;
    static const uint32_t kWaitForAckMilli = 2000;

    static uint8_t tx_count;
    static uint8_t rx_count;
    static uint8_t command_success_count;
    bool lithium_transmit_enabled;

    Mailbox_Params message_mailbox_params;
    Mailbox_Handle message_mailbox_handle;
    Mailbox_Params command_response_mailbox_params;
    Mailbox_Handle command_response_mailbox_handle;
    // TODO(wschuetz): Review the number of the payloads that can be stored in
    // the mailbox at any one time.
    static const uint8_t kMaxNumberOfPayloads = 2;

    LithiumConfiguration lithium_config;
    Uart uart;

    Lithium();
    Uart* GetUart();
    void SetTransmitEnabled(bool lithium_enabled);
};

#endif  // SRC_TELECOMMS_LITHIUM_H_
