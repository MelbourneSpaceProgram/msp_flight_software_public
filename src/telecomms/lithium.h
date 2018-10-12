#ifndef SRC_TELECOMMS_LITHIUM_H_
#define SRC_TELECOMMS_LITHIUM_H_

#include <src/board/uart/uart.h>
#include <ti/sysbios/knl/Mailbox.h>

typedef struct LithiumTelemetry LithiumTelemetry;
class LithiumCommand;
class LithiumEnableCommand;
class RunnableLithiumListener;
class PayloadProcessor;
class LithiumConfiguration;

class Lithium {
    friend class PayloadProcessor;         // For incrementing command success
    friend class RunnableLithiumListener;  // For incrementing received count
    friend class LithiumEnableCommand;
    friend class RunnableContinuousTransmitShutoff;  // For calculating
                                                     // transmission rate

   public:
    static const uint8_t kLithiumHeaderSize = 8;
    static const uint8_t kLithiumTailSize = 2;
    static const uint8_t kLithiumSyncSize = 2;

    // TODO(dingbenjamin): Reduce and tighten bounds on max received size
    static const uint16_t kMaxOutgoingCommandSize = 300;
    static const uint16_t kMaxReceivedUartSize = 310;
    static const uint16_t kMaxReceivedUplinkSize =
        kMaxReceivedUartSize - kLithiumHeaderSize - kLithiumTailSize;
    static const uint16_t kMaxReceivedLithiumResponseSize = 50;
    static const uint16_t kReceivedPacketSize = 265;
    static const uint16_t kMaxDownlinkSize = 255;

    static const byte kSyncCharOne = 0x48;
    static const byte kSyncCharTwo = 0x65;
    static const byte kDirectionIntoLithium = 0x10;
    static const byte kDirectionOutOfLithium = 0x20;

    static const uint16_t kInterCommandTimeMilli = 250;

    static Lithium* GetInstance();  // Initial call is not thread safe
    static uint8_t GetTxCounter();
    static uint8_t GetRxCounter();
    static uint8_t GetCommandSuccessCounter();
    Mailbox_Handle GetUplinkMailbox() const;
    Mailbox_Handle GetHeaderMailbox() const;
    Mailbox_Handle GetCommandResponseMailbox() const;
    Mailbox_Handle GetReceiveMailbox() const;
    LithiumTelemetry ReadLithiumTelemetry() const;
    LithiumConfiguration ReadLithiumConfiguration() const;
    bool IsTransmitEnabled();
    bool DoCommand(LithiumCommand* command) const;

   private:
    static Lithium* instance;
    static const uint32_t kUartReadTimeoutMilli = 300;
    static const uint32_t kUartWriteTimeoutMilli = 500;
    static const uint32_t kWaitForAckMilli = 3000;
    static const uint32_t kWaitForReplyPayloadMilli = 5000;

    static uint8_t tx_count;
    static uint8_t rx_count;
    static uint8_t command_success_count;
    bool lithium_transmit_enabled;

    Mailbox_Params uplink_mailbox_params;
    Mailbox_Handle uplink_mailbox_handle;
    Mailbox_Params header_mailbox_params;
    Mailbox_Handle header_mailbox_handle;
    Mailbox_Params command_response_mailbox_params;
    Mailbox_Handle command_response_mailbox_handle;
    // TODO(wschuetz): Review the number of the payloads that can be stored in
    // the mailbox at any one time.
    static const uint8_t kMaxNumberOfPayloads = 2;

    Uart uart;

    Lithium();
    Uart* GetUart();
    void SetTransmitEnabled(bool lithium_enabled);
};

#endif  // SRC_TELECOMMS_LITHIUM_H_
