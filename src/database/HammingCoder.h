#ifndef SRC_DATABASE_HAMMINGCODER_H_
#define SRC_DATABASE_HAMMINGCODER_H_

#include <src/util/data_types.h>

typedef struct HammingEncodedByte {
    byte codewords[2];
} HammingEncodedByte;

typedef struct HammingDecodedByte {
    byte data;
    bool valid;
} HammingDecodedByte;

class HammingCoder {
    // TODO(hugorilla): pass method arguments by constant reference (where
    // appropriate)
   public:
    static HammingEncodedByte Encode(byte data);
    static void EncodeByteArray(byte *output, uint32_t output_length,
                                byte *input, uint32_t input_length);
    static HammingDecodedByte Decode(HammingEncodedByte hamming_encoded_byte);
    static void DecodeByteArray(byte *output, uint32_t output_length,
                                bool *valid_output,
                                uint32_t valid_output_length, byte *input,
                                uint32_t input_length);

   private:
    static byte EncodeNibble(byte nibble);
    static bool ValidateCodeword(byte &codeword);
    static byte ExtractAndJoinData(byte c0, byte c1);
};

#endif  // SRC_DATABASE_HAMMINGCODER_H_
