#ifndef SRC_TELECOMMS_LITHIUM_H_
#define SRC_TELECOMMS_LITHIUM_H_

#include <src/board/uart/uart.h>
#include <ti/sysbios/gates/GateMutexPri.h>
#include <ti/sysbios/knl/Mailbox.h>

class TransmitPayload;

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
    enum kLithiumState {
        kLithiumTempCriticalLow = 0,
        kLithiumTempNominal = 1,
        kLithiumTempCriticalHigh = 2
    };

    static constexpr int8_t kLithiumTempOperationalMax = 60;
    static constexpr int8_t kLithiumTempOperationalMin = -20;
    static constexpr int8_t kHysteresis = 5;
    static constexpr int8_t kLithiumTempOperationalNominal = 20;

    static constexpr uint8_t kLithiumHeaderSize = 8;
    static constexpr uint8_t kLithiumTailSize = 2;
    static constexpr uint8_t kLithiumSyncSize = 2;

    // TODO(dingbenjamin): Reduce and tighten bounds on max received size
    static constexpr uint16_t kMaxOutgoingCommandSize = 300;
    static constexpr uint16_t kMaxReceivedUartSize = 310;
    static constexpr uint16_t kMaxReceivedUplinkSize =
        kMaxReceivedUartSize - kLithiumHeaderSize - kLithiumTailSize;
    static constexpr uint16_t kMaxReceivedLithiumResponseSize = 50;
    static constexpr uint16_t kReceivedPacketSize = 265;
    static constexpr uint16_t kMaxDownlinkSize = 255;

    static constexpr byte kSyncCharOne = 0x48;
    static constexpr byte kSyncCharTwo = 0x65;
    static constexpr byte kDirectionIntoLithium = 0x10;
    static constexpr byte kDirectionOutOfLithium = 0x20;

    static constexpr uint16_t kInterCommandTimeMilli = 250;

    enum LithiumShutoffCondition {
        kLithiumOnCondition = 0x0,
        kCriticalTempCondition = 0x1,
        kContinuousTransmitCondition = (0x1 << 1),
    };

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
    bool Transmit(TransmitPayload* transmit_payload);
    bool DoCommand(LithiumCommand* command) const;
    // TODO(dingbenjamin): Make this function private if possible, friend unit
    // tests
    void SetTransmitEnabled(bool lithium_enabled);

    void UpdateState();
    void LockState(LithiumShutoffCondition condition);
    void UnlockState(LithiumShutoffCondition condition);
    bool IsStateLocked(LithiumShutoffCondition condition);
    void ForceUnlock();

    static bool IsTransmitting();

   private:
    static void PreTransmit();
    static void PostTransmit();
    static Lithium* instance;
    static constexpr uint32_t kUartReadTimeoutMilli = 300;
    static constexpr uint32_t kUartWriteTimeoutMilli = 500;
    static constexpr uint32_t kWaitForAckMilli = 3000;
    static constexpr uint32_t kWaitForReplyPayloadMilli = 5000;

    static uint8_t tx_count;
    static uint8_t rx_count;
    static uint8_t command_success_count;
    static IArg power_key;

    Uart uart;
    bool lithium_transmit_enabled;
    kLithiumState state;
    uint8_t lock;

    Mailbox_Params uplink_mailbox_params;
    Mailbox_Handle uplink_mailbox_handle;
    Mailbox_Params header_mailbox_params;
    Mailbox_Handle header_mailbox_handle;
    Mailbox_Params command_response_mailbox_params;
    Mailbox_Handle command_response_mailbox_handle;

    static bool currently_transmitting;

    // TODO(wschuetz): Review the number of the payloads that can be stored in
    // the mailbox at any one time.
    static constexpr uint8_t kMaxNumberOfPayloads = 2;

    Lithium();
    Uart* GetUart();
};

#endif  // SRC_TELECOMMS_LITHIUM_H_
