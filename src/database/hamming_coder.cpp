#include <external/etl/exception.h>
#include <src/database/hamming_coder.h>

HammingEncodedByte HammingCoder::Encode(byte data) {
    HammingEncodedByte hamming_encoded_byte;
    byte nibble0, nibble1;

    // split the byte into two nibbles
    nibble0 = data >> 4;
    nibble1 = data << 4;
    nibble1 = nibble1 >> 4;

    hamming_encoded_byte.codewords[0] = HammingCoder::EncodeNibble(nibble0);

    hamming_encoded_byte.codewords[1] = HammingCoder::EncodeNibble(nibble1);

    return hamming_encoded_byte;
}

void HammingCoder::EncodeByteArray(byte *output, uint32_t output_length,
                                   byte *input, uint32_t input_length) {
    if (output_length != input_length * 2) {
        etl::exception e("HammingCoder::EncodeByteArray invalid arguments",
                         __FILE__, __LINE__);
        throw e;
    }
    for (uint32_t i = 0; i < input_length; i++) {
        HammingEncodedByte output_byte = Encode(input[i]);
        output[i * 2] = output_byte.codewords[0];
        output[i * 2 + 1] = output_byte.codewords[1];
    }
}

byte HammingCoder::EncodeNibble(byte nibble) {
    byte encoded_nibble;
    byte data1, data2, data3, data4;
    byte parity1, parity2, parity3, parity4;

    data1 = (nibble & 8) >> 3;
    data2 = (nibble & 4) >> 2;
    data3 = (nibble & 2) >> 1;
    data4 = nibble & 1;

    parity1 = data1 ^ data2 ^ data4;
    parity2 = data1 ^ data3 ^ data4;
    parity3 = data2 ^ data3 ^ data4;
    parity4 = data1 ^ data2 ^ data3 ^ data4 ^ parity1 ^ parity2 ^ parity3;

    encoded_nibble = (parity1 << 7) + (parity2 << 6) + (data1 << 5) +
                     (parity3 << 4) + (data2 << 3) + (data3 << 2) +
                     (data4 << 1) + parity4;

    return encoded_nibble;
}

HammingDecodedByte HammingCoder::Decode(
    HammingEncodedByte hamming_encoded_byte) {
    HammingDecodedByte hamming_decoded_byte;

    byte c0 = hamming_encoded_byte.codewords[0];
    byte c1 = hamming_encoded_byte.codewords[1];

    // check that the codewords are valid, and if necessary correct single-bit
    // errors
    if (ValidateCodeword(c0) && ValidateCodeword(c1)) {
        hamming_decoded_byte.valid = true;
        hamming_decoded_byte.data = HammingCoder::ExtractAndJoinData(c0, c1);
    } else {
        hamming_decoded_byte.valid = false;
        hamming_decoded_byte.data = 0;
    }

    return hamming_decoded_byte;
}

void HammingCoder::DecodeByteArray(byte *output, uint32_t output_length,
                                   bool *valid_output, byte *input,
                                   uint32_t input_length) {
    if ((output_length * 2 != input_length)) {
        etl::exception e("HammingCoder::DecodeByteArray invalid arguments",
                         __FILE__, __LINE__);
        throw e;
    }
    for (uint32_t i = 0; i < output_length; i++) {
        HammingEncodedByte input_byte;
        input_byte.codewords[0] = input[i * 2];
        input_byte.codewords[1] = input[i * 2 + 1];
        HammingDecodedByte output_byte = Decode(input_byte);
        output[i] = output_byte.data;
        valid_output[i] = output_byte.valid;
    }
}

byte HammingCoder::ExtractAndJoinData(byte c0, byte c1) {
    byte d1, d2, d3, d4;
    byte joined_data = 0;

    // extract data bits from codewords and put them in the correct position
    d1 = (c0 & 0b00100000) >> 5;
    d2 = (c0 & 0b00001000) >> 3;
    d3 = (c0 & 0b00000100) >> 2;
    d4 = (c0 & 0b00000010) >> 1;
    joined_data += (d1 << 7) + (d2 << 6) + (d3 << 5) + (d4 << 4);

    d1 = (c1 & 0b00100000) >> 5;
    d2 = (c1 & 0b00001000) >> 3;
    d3 = (c1 & 0b00000100) >> 2;
    d4 = (c1 & 0b00000010) >> 1;
    joined_data += (d1 << 3) + (d2 << 2) + (d3 << 1) + d4;

    return joined_data;
}

bool HammingCoder::ValidateCodeword(byte &codeword) {
    byte d1, d2, d3, d4;
    d1 = (codeword & 0b00100000) >> 5;
    d2 = (codeword & 0b00001000) >> 3;
    d3 = (codeword & 0b00000100) >> 2;
    d4 = (codeword & 0b00000010) >> 1;
    HammingEncodedByte check =
        HammingCoder::Encode((d1 << 7) + (d2 << 6) + (d3 << 5) + (d4 << 4));

    if (codeword == check.codewords[0]) {
        // the data is uncorrupted.
        return true;
    }

    // calculate the number of ones in `codeword`.
    byte temp = codeword;
    uint32_t num_ones = 0;
    while (temp) {
        num_ones += temp % 2;
        temp = temp >> 1;
    }

    if (num_ones % 2 == 0) {
        // the Hamming distance of `codeword` from a valid codeword is 2, so
        // the codeword is unreadable.
        return false;
    } else {
        // the Hamming distance of `codeword` from a valid codeword is 1, so
        // the codeword can be corrected.
        byte parity1, parity2, parity3;
        byte check_parity1, check_parity2, check_parity3;

        parity1 = (codeword & 0b10000000) >> 7;
        parity2 = (codeword & 0b01000000) >> 6;
        parity3 = (codeword & 0b00010000) >> 4;

        check_parity1 = (check.codewords[0] & 0b10000000) >> 7;
        check_parity2 = (check.codewords[0] & 0b01000000) >> 6;
        check_parity3 = (check.codewords[0] & 0b00010000) >> 4;

        byte syndrome =
            ((parity3 << 2) + (parity2 << 1) + parity1) ^
            ((check_parity3 << 2) + (check_parity2 << 1) + check_parity1);
        // flip the bit indexed by the syndrome
        codeword = codeword ^ (1 << (8 - syndrome));

        return true;
    }
}
