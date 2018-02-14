#include <src/database/HammingCoder.h>
#include <test_runners/unity.h>

void TestHamming() {
    HammingEncodedByte hamming_encoded_byte;

    // test ENCODING
    byte data = 0b01001010;
    hamming_encoded_byte = HammingCoder::Encode(data);
    TEST_ASSERT_EQUAL_UINT8(0b10011001, hamming_encoded_byte.codewords[0]);
    TEST_ASSERT_EQUAL_UINT8(0b10110100, hamming_encoded_byte.codewords[1]);

    // test DECODING
    HammingDecodedByte hamming_decoded_byte;

    // decoding of uncorrupted bytes
    hamming_encoded_byte.codewords[0] = 0b11100001;
    hamming_encoded_byte.codewords[1] = 0b10011001;
    hamming_decoded_byte = HammingCoder::Decode(hamming_encoded_byte);
    TEST_ASSERT_TRUE(hamming_decoded_byte.valid);
    TEST_ASSERT_EQUAL_UINT8(0b10000100, hamming_decoded_byte.data);

    // decoding of bytes each with a single bit error
    hamming_encoded_byte.codewords[0] = 0b11110001;
    hamming_encoded_byte.codewords[1] = 0b10011011;
    hamming_decoded_byte = HammingCoder::Decode(hamming_encoded_byte);
    TEST_ASSERT_TRUE(hamming_decoded_byte.valid);
    TEST_ASSERT_EQUAL_UINT8(0b10000100, hamming_decoded_byte.data);

    // decoding of irrecoverable data
    hamming_encoded_byte.codewords[0] = 0b00100001;
    hamming_encoded_byte.codewords[1] = 0b10011001;
    hamming_decoded_byte = HammingCoder::Decode(hamming_encoded_byte);
    TEST_ASSERT_FALSE(hamming_decoded_byte.valid);
    TEST_ASSERT_EQUAL_UINT8(0b00000000, hamming_decoded_byte.data);
}
