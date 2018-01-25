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
    static Lithium* GetInstance();  // Initial call is not thread safe
    void SendCommand(LithiumCommand* command) const;
    const LithiumConfiguration& GetLithiumConfig() const;
    void SetLithiumConfig(const LithiumConfiguration& lithium_config);
    static void* InitLithiumTestStream();

   private:
    Lithium();

    const static uint16_t kMaxCommandSize = 300;
    static Lithium* instance;
    uint8_t tx_counter;
    uint8_t total_rx_counter;
    uint8_t valid_rx_counter;
    LithiumConfiguration lithium_config;
    UartConfiguration uart_config;
    Uart uart;
};

#endif  // SRC_TELECOMMS_LITHIUM_H_
