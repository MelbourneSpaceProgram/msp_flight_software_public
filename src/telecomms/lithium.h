#ifndef SRC_TELECOMMS_LITHIUM_H_
#define SRC_TELECOMMS_LITHIUM_H_

#include <src/telecomms/lithium_commands/get_configuration_command.h>
#include <src/telecomms/lithium_commands/lithium_command.h>
#include <src/telecomms/lithium_commands/no_op_command.h>
#include <src/telecomms/lithium_commands/reset_system_command.h>
#include <src/telecomms/lithium_commands/transmit_command.h>
#include <src/telecomms/lithium_configuration.h>
#include <src/telecomms/msp_payloads/test_payload.h>
#include <src/telecomms/msp_payloads/transmit_packet.h>
#include <src/telecomms/msp_payloads/transmit_payload.h>
#include <src/uart/uart.h>
#include <src/uart/uart_configuration.h>

class Lithium {
   public:
    enum LithiumReturnCode {
        kInvalidReceivedSync,
        kInvalidReceivedDirection,
        kInvalidReceivedCommandCode,
        kInvalidReceivedSize,
        kOverflowReceivedSize,
        kInvalidReceivedChecksum,
        kInvalidCommandSize,
        kUartWriteError,
        kUartReadError,
        kNack,
        kSuccess
    };

    static Lithium* GetInstance();  // Initial call is not thread safe
    // TODO(dingbenjamin): Find a way to make this function private but still
    // unit test
    static LithiumReturnCode CheckIncomingHeader(LithiumCommand* command,
                                                 byte* received,
                                                 uint16_t received_size);
    static uint16_t GetReplyPayloadSize(byte* received);
    // The raw bytes are copied into output_buffer on success
    LithiumReturnCode PerformTransaction(LithiumCommand* command,
                                         byte* read_buffer) const;
    const LithiumConfiguration& GetLithiumConfig() const;
    void SetLithiumConfig(const LithiumConfiguration& lithium_config);

    static const uint16_t kMaxCommandSize = 300;
    static const uint16_t kMaxReceivedSize = 300;

   private:
    Lithium();
    LithiumReturnCode PerformWriteTransaction(LithiumCommand* command) const;
    LithiumReturnCode PerformReadTransaction(LithiumCommand* command,
                                             byte* read_buffer) const;

    static Lithium* instance;
    uint8_t tx_counter;
    uint8_t total_rx_counter;
    uint8_t valid_rx_counter;
    LithiumConfiguration lithium_config;
    UartConfiguration uart_config;
    Uart uart;
};

#endif  // SRC_TELECOMMS_LITHIUM_H_
