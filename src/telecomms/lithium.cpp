#include <src/config/board_definitions.h>
#include <src/messages/serialised_message.h>
#include <src/telecomms/lithium.h>
#include <src/telecomms/lithium_commands/lithium_command.h>
#include <src/util/data_types.h>

Lithium* Lithium::instance = NULL;

Lithium::Lithium() : lithium_config(), uart(UARTA2) {
    uart.SetBaudRate(Uart::kBaud9600)->Open();
}

Lithium::LithiumReturnCode Lithium::PerformTransaction(
    LithiumCommand* command, byte* read_buffer) const {
    // TODO(dingbenjamin): Set uninterruptible
    Lithium::LithiumReturnCode write_status = PerformWriteTransaction(command);
    if (write_status != kSuccess) {
        return write_status;
    }

    Lithium::LithiumReturnCode read_status =
        PerformReadTransaction(command, read_buffer);
    if (read_status != kSuccess || command->GetReplyPayloadSize() == 0) {
        return read_status;
    } else if (command->GetReplyPayloadSize() !=
               GetReplyPayloadSize(read_buffer)) {
        return kInvalidReceivedSize;
    } else {
        return kSuccess;
    }
}

Lithium::LithiumReturnCode Lithium::PerformWriteTransaction(
    LithiumCommand* command) const {
    uint16_t serialised_size = command->GetSerialisedSize();
    if (serialised_size >= kMaxCommandSize) {
        return kInvalidCommandSize;
    }
    byte command_buffer[kMaxCommandSize];
    SerialisedMessage serial_command = command->SerialiseTo(command_buffer);
    if (!uart.PerformWriteTransaction(serial_command.GetBuffer(),
                                      serial_command.GetSize())) {
        return kUartWriteError;
    }
    return kSuccess;
}

Lithium::LithiumReturnCode Lithium::PerformReadTransaction(
    LithiumCommand* command, byte* read_buffer) const {
    if (!uart.PerformReadTransaction(read_buffer, kMaxReceivedSize)) {
        return kUartReadError;
    }
    // TODO(dingbejamin): Figure out how to get reply_size
    // return CheckIncomingHeader(command, read_buffer, reply_size);
    return kSuccess;
}

const LithiumConfiguration& Lithium::GetLithiumConfig() const {
    return lithium_config;
}

void Lithium::SetLithiumConfig(const LithiumConfiguration& lithium_config) {
    this->lithium_config = lithium_config;
}

// GetInstance() is not thread safe on first initialisation
// It is recommended that it be initialised from a single thread
Lithium* Lithium::GetInstance() {
    if (instance == NULL) {
        instance = new Lithium();
    }
    return instance;
}

Lithium::LithiumReturnCode Lithium::CheckIncomingHeader(
    LithiumCommand* command, byte* received, uint16_t received_size) {
    // Check size
    if (received_size < LithiumCommand::kLithiumCommandHeaderSize) {
        return kInvalidReceivedSize;
    }
    // Check sync chars
    if (!(received[0] == LithiumCommand::kSyncCharOne &&
          received[1] == LithiumCommand::kSyncCharTwo)) {
        return kInvalidReceivedSync;
    }
    // Check direction
    if (received[2] != LithiumCommand::kDirectionOutOfLithium) {
        return kInvalidReceivedDirection;
    }
    // Check command code
    if (received[3] != command->GetCommandCode()) {
        return kInvalidReceivedCommandCode;
    }
    // Check checksum over non sync bytes (4 bytes)
    byte checksum[2];
    LithiumCommand::CalcChecksum(checksum, received + 2, 4);
    if (!(checksum[0] == received[6] && checksum[1] == received[7])) {
        return kInvalidReceivedChecksum;
    }
    // Check payload sizing / payload
    if (command->GetReplyPayloadSize() == 0) {
        if (received[4] == 0x0a && received[5] == 0x0a) {
            return kSuccess;
        } else if (received[4] == 0xff && received[5] == 0xff) {
            return kNack;
        }
    }
    if (command->GetReplyPayloadSize() > kMaxReceivedSize) {
        return kOverflowReceivedSize;
    }
    return kSuccess;
}

uint16_t Lithium::GetReplyPayloadSize(byte* received) {
    return (static_cast<uint16_t>(received[4] << 8) | received[5]);
}
