#ifndef SRC_TELECOMMS_LITHIUM_UTILS_H_
#define SRC_TELECOMMS_LITHIUM_UTILS_H_

#include <src/util/data_types.h>

class LithiumUtils {
   public:
    static bool IsValidHeader(const byte* header);
    static bool CheckTailChecksum(const byte* payload,
                                  uint16_t lithium_payload_size);
    static bool IsAck(const byte* header);
    static bool IsNack(const byte* header);
    static uint16_t GetPayloadSize(const byte* header);
    static void CalcChecksum(byte* checksum, const byte* data,
                             const uint16_t data_size);
    static uint8_t GetCommandCode(const byte* header);
};

#endif  // SRC_TELECOMMS_LITHIUM_UTILS_H_
