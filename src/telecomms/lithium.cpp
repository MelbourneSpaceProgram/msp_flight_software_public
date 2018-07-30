#include <external/etl/exception.h>
#include <src/board/board.h>
#include <src/messages/serialised_message.h>
#include <src/telecomms/lithium.h>
#include <src/telecomms/lithium_commands/get_configuration_command.h>
#include <src/telecomms/lithium_commands/lithium_command.h>
#include <src/telecomms/lithium_commands/no_op_command.h>
#include <src/telecomms/lithium_commands/reset_system_command.h>
#include <src/telecomms/lithium_commands/transmit_command.h>
#include <src/telecomms/lithium_commands/write_flash_command.h>
#include <src/telecomms/lithium_utils.h>
#include <src/util/data_types.h>
#include <ti/drivers/GPIO.h>
#include <ti/sysbios/knl/Clock.h>
#include <ti/sysbios/knl/Task.h>
#include <xdc/runtime/Log.h>

Lithium* Lithium::instance = NULL;

Lithium::Lithium()
    : lithium_config(), uart(TELECOMS), lithium_transmit_enabled(true) {
    // Ensure Lithium is not in reset
    GPIO_write(nCOMMS_RST, 1);

    uart.SetBaudRate(Uart::kBaud9600)
        ->SetReadMode(UART_MODE_BLOCKING)
        ->SetWriteMode(UART_MODE_BLOCKING)
        ->SetWriteTimeout(kUartWriteTimeoutMilli * 1000 / Clock_tickPeriod)
        ->Open();

    Mailbox_Params_init(&command_response_mailbox_params);
    command_response_mailbox_handle = Mailbox_create(
        kLithiumHeaderSize, 1, &command_response_mailbox_params, NULL);
    if (command_response_mailbox_handle == NULL) {
        etl::exception e("Unable to create Lithium command response mailbox",
                         __FILE__, __LINE__);
        throw e;
    }

    Mailbox_Params_init(&message_mailbox_params);
    message_mailbox_handle = Mailbox_create(
        kMaxReceivedSize, kMaxNumberOfPayloads, &message_mailbox_params, NULL);
    if (message_mailbox_handle == NULL) {
        etl::exception e("Unable to create Lithium message response mailbox",
                         __FILE__, __LINE__);
        throw e;
    }
}

bool Lithium::DoCommand(LithiumCommand* command) const {
    if (!lithium_transmit_enabled) {
        Log_info0("Attempted to transmit, but transmit disabled");
        return false;
    }

    uint16_t serialised_size = command->GetSerialisedSize();
    if (serialised_size >= kMaxCommandSize) {
        return false;
    }
    byte command_buffer[kMaxCommandSize];
    SerialisedMessage serial_command = command->SerialiseTo(command_buffer);

    if (!uart.PerformWriteTransaction(serial_command.GetBuffer(),
                                      serial_command.GetSize())) {
        return false;
    }
    byte ack_buffer[kLithiumHeaderSize] = {NULL};
    if (!Mailbox_pend(command_response_mailbox_handle, &ack_buffer,
                      kWaitForAckMilli * 1000 / Clock_tickPeriod)) {
        // Timed out waiting for a response
        return false;
    }
    // TODO(dingbenjamin): Figure out why this is needed
    Task_sleep(kInterCommandTimeMilli * 1000 / Clock_tickPeriod);
    if (LithiumUtils::IsValidHeader(ack_buffer) &&
        LithiumUtils::GetCommandCode(ack_buffer) == command->GetCommandCode() &&
        LithiumUtils::IsAck(ack_buffer)) {
        return true;
    } else if (LithiumUtils::IsValidHeader(ack_buffer) &&
               LithiumUtils::GetCommandCode(ack_buffer) ==
                   command->GetCommandCode() &&
               LithiumUtils::IsNack(ack_buffer)) {
        // TODO(dingbenjamin): Received Nack, write code to back off and try
        // again
        return false;
    } else {
        // TODO(dingbenjamin): There is something wrong with the received
        // header, write code to reset the lithium
        return false;
    }
}

Lithium* Lithium::GetInstance() {
    if (instance == NULL) {
        instance = new Lithium();
    }
    return instance;
}

const LithiumConfiguration& Lithium::GetLithiumConfig() const {
    return lithium_config;
}

void Lithium::SetLithiumConfig(const LithiumConfiguration& lithium_config) {
    this->lithium_config = lithium_config;
}

Uart* Lithium::GetUart() { return &uart; }

Mailbox_Handle Lithium::GetMessageMailbox() const {
    return message_mailbox_handle;
}

Mailbox_Handle Lithium::GetCommandResponseMailbox() const {
    return command_response_mailbox_handle;
}

void Lithium::SetTransmitEnabled(bool lithium_enabled) {
    lithium_transmit_enabled = lithium_enabled;
}

bool Lithium::IsTransmitEnabled() { return lithium_transmit_enabled; }
