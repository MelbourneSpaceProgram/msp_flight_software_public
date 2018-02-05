#include <src/telecomms/lithium_commands/lithium_command.h>

LithiumCommand::LithiumCommand(byte command_code, Message *lithium_payload)
    : lithium_payload(lithium_payload), command_code(command_code) {}

SerialisedMessage LithiumCommand::SerialiseTo(byte *serial_buffer) {
    SerialisedMessageBuilder builder(serial_buffer, GetSerialisedSize());
    BuildHeader(&builder);
    if (lithium_payload != NULL) {
        builder.AddMessage(lithium_payload);
        BuildTail(&builder);
    }
    return builder.Build();
}

// Documentation on building a Lithium header can be found in the the Radio
// Interface Manual at CS1-TXT-14
void LithiumCommand::BuildHeader(SerialisedMessageBuilder *builder) {
    byte header[kLithiumCommandHeaderSize];
    uint16_t serial_payload_size = GetLithiumPayloadSize();

    // Sync characters,
    header[0] = LithiumCommand::kSyncCharOne;
    header[1] = LithiumCommand::kSyncCharTwo;
    // Direction of communication
    header[2] = LithiumCommand::kDirectionIntoLithium;
    // Command code
    header[3] = command_code;
    // Payload Size
    header[4] = serial_payload_size >> 8;
    header[5] = serial_payload_size & 0xFF;
    // Checksum
    byte *checksum_data = header + 2;
    uint8_t checksum_data_size = 4;  // Header minus sync chars / checksum bits
    byte *checksum_location = header + 6;
    CalcChecksum(checksum_location, checksum_data, checksum_data_size);

    for (uint8_t i = 0; i < LithiumCommand::kLithiumCommandHeaderSize; i++) {
        builder->AddData<byte>(header[i]);
    }
}

void LithiumCommand::BuildTail(SerialisedMessageBuilder *builder) {
    byte tail_checksum[2];
    // Checksum is calculated over all bytes except the first two sync chars
    byte *checksum_data = builder->GetSerialisedMessageBuffer() + 2;

    // Calculated across current buffer minus 2 sync chars 'He'
    uint16_t checksum_data_size = builder->GetSerialisedLength() - 2;

    CalcChecksum(tail_checksum, checksum_data, checksum_data_size);
    builder->AddData<byte>(tail_checksum[0]);
    builder->AddData<byte>(tail_checksum[1]);
}

// Documentation on building a Lithium checksum can be found in the the Radio
// Interface Manual at CS1-TXT-14
void LithiumCommand::CalcChecksum(byte *checksum, byte *data,
                                  uint16_t data_size) {
    uint8_t check_byte_a = 0;
    uint8_t check_byte_b = 0;
    uint16_t i;

    for (i = 0; i < data_size; i++) {
        check_byte_a = check_byte_a + data[i];
        check_byte_b = check_byte_b + check_byte_a;
    }

    checksum[0] = check_byte_a;
    checksum[1] = check_byte_b;
}

uint16_t LithiumCommand::GetSerialisedSize() const {
    uint16_t payload_size = GetLithiumPayloadSize();
    if (payload_size == 0) {
        return kLithiumCommandHeaderSize;
    } else {
        return kLithiumCommandHeaderSize + payload_size +
               kLithiumCommandTailSize;
    }
}
