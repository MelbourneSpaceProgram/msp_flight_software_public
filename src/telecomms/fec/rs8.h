#ifndef SRC_TELECOMMS_FEC_REED_SOLOMON_RS8_H_
#define SRC_TELECOMMS_FEC_REED_SOLOMON_RS8_H_

#include <src/util/data_types.h>

class Rs8 {
   public:
    static constexpr uint32_t kDataLength = 223;
    static constexpr uint32_t kParityLength = 32;
    static constexpr uint32_t kBlockLength = kDataLength + kParityLength;

    static void Encode(uint8_t *data, uint8_t *parity);
    // Returns -1 if uncorrectable
    static int32_t Decode(uint8_t *data, int32_t *eras_pos, int32_t no_eras);

   private:
    static constexpr uint8_t kMn = 8;
    static constexpr uint8_t kNn = 255;
    static constexpr uint8_t kNRoots = 32;
    static constexpr uint8_t kFcr = 112;
    static constexpr uint8_t kPrim = 11;
    static constexpr uint8_t kIPrim = 116;
    static constexpr uint8_t kA0 = kNn;

    // TODO(dingbenjamin): Make this a constexpr
    // CCSDS Alpha to lookup table
    static const uint8_t alpha_to[];
    // CCSDS Index of lookup table
    static const uint8_t index_of[];
    // CCSDS generator polynomial lookup table
    static const uint8_t gen_poly[];
};

#endif  // SRC_TELECOMMS_FEC_REED_SOLOMON_RS8_H_
