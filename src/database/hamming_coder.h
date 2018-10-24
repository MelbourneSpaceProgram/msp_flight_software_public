#ifndef SRC_DATABASE_HAMMINGCODER_H_
#define SRC_DATABASE_HAMMINGCODER_H_

#include <src/util/data_types.h>
#include <src/util/matrix.h>

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
                                bool *valid_output, byte *input,
                                uint32_t input_length);

   private:
    static const uint32_t kGenerator2dArray[4][8];
    static const uint32_t generator_data[4][8];
    static uint32_t kParityCheck2dArray[4][8];
    static uint32_t parity_check_data[4][8];

    static const Matrix<4, 8> kGeneratorMatrix;
    static const Matrix<4, 8> kParityCheckMatrix;

    static byte EncodeNibble(byte nibble);
    static bool ValidateCodeword(byte &codeword);
    static byte ExtractAndJoinData(byte c0, byte c1);
};

#endif  // SRC_DATABASE_HAMMINGCODER_H_
