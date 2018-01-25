#include <src/config/board_definitions.h>
#include <src/messages/serialised_message.h>
#include <src/telecomms/lithium.h>
#include <src/telecomms/lithium_commands/lithium_command.h>
#include <src/util/data_types.h>

Lithium* Lithium::instance = NULL;

Lithium::Lithium()
    : lithium_config(),
      uart_config(UartConfiguration::kBaud9600),
      uart(uart_config, UARTA2) {}

void Lithium::SendCommand(LithiumCommand* command) const {
    uint16_t serial_size = command->GetSerialisedSize();
    if (serial_size < kMaxCommandSize) {
        byte command_buffer[serial_size];
        SerialisedMessage serial_command = command->SerialiseTo(command_buffer);
        uart.PerformWriteTransaction(serial_command.GetBuffer(),
                                     serial_command.GetSize());
    } else {
        // TODO(dingbenjamin): Throw an error
    }
}

const LithiumConfiguration& Lithium::GetLithiumConfig() const {
    return lithium_config;
}

void Lithium::SetLithiumConfig(const LithiumConfiguration& lithium_config) {
    this->lithium_config = lithium_config;
}

// GetInstance() is not thread safe on first initialisation
// It is recommended that it be called from a single thread only the first time
Lithium* Lithium::GetInstance() {
    if (instance == NULL) {
        instance = new Lithium();
    }
    return instance;
}
