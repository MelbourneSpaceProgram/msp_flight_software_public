#include <src/telecomms/lithium_utils.h>

bool LithiumUtils::IsValidHeader(const byte* received) {
    // Check sync chars
    if (!(received[0] == Lithium::kSyncCharOne &&
          received[1] == Lithium::kSyncCharTwo)) {
        return false;
    }
    // Check direction
    if (received[2] != Lithium::kDirectionOutOfLithium) {
        return false;
    }
    // Check checksum over non sync bytes (4 bytes)
    byte checksum[2];
    LithiumUtils::CalcChecksum(checksum, received + 2, 4);
    if (!(checksum[0] == received[6] && checksum[1] == received[7])) {
        return false;
    }
    return true;
}

uint16_t LithiumUtils::GetPayloadSize(const byte* header) {
    // Ack (See Lithium Radio Interface Manual)
    if (header[4] == 0x0a && header[5] == 0x0a) {
        return 0;
    }
    // Nack (See Lithium Radio Interface Manual)
    if (header[4] == 0xff && header[5] == 0xff) {
        return 0;
    }
    return (static_cast<uint16_t>(header[4] << 8) | header[5]);
}

// Documentation on building a Lithium checksum can be found in the the Radio
// Interface Manual at CS1-TXT-14
void LithiumUtils::CalcChecksum(byte* checksum, const byte* data,
                                const uint16_t data_size) {
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

uint8_t LithiumUtils::GetCommandCode(const byte* header) { return header[3]; }
