#ifndef SRC_TELECOMMS_LITHIUM_UTILS_H_
#define SRC_TELECOMMS_LITHIUM_UTILS_H_

#include <src/telecomms/lithium.h>
#include <src/telecomms/lithium_commands/lithium_command.h>

class LithiumUtils {
   public:
    static bool IsValidHeader(const byte* header);
    static bool IsAck(const byte* header);
    static uint16_t GetPayloadSize(const byte* header);
    static void CalcChecksum(byte* checksum, const byte* data,
                             const uint16_t data_size);
    static uint8_t GetCommandCode(const byte* header);
};

#endif  // SRC_TELECOMMS_LITHIUM_UTILS_H_
