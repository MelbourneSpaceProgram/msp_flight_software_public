#include <external/etl/exception.h>
#include <src/config/unit_tests.h>
#include <src/database/eeprom.h>
#include <src/database/hamming_coder.h>
#include <src/util/data_types.h>
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

    byte byte_array[2] = {0b10101010, 0b10010010};
    byte encoded_byte_array[4], decoded_byte_array[2];
    bool invalid_bytes[2];

    bool failed = false;
    try {
        HammingCoder::EncodeByteArray(encoded_byte_array, 2, byte_array, 4);
    } catch (etl::exception& e) {
        failed = true;
    }
    TEST_ASSERT_TRUE(failed);

    failed = false;
    try {
        HammingCoder::DecodeByteArray(decoded_byte_array, 4, invalid_bytes,
                                      encoded_byte_array, 2);
    } catch (etl::exception& e) {
        failed = true;
    }
    TEST_ASSERT_TRUE(failed);

    HammingCoder::EncodeByteArray(encoded_byte_array, 4, byte_array, 2);
    HammingCoder::DecodeByteArray(decoded_byte_array, 2, invalid_bytes,
                                  encoded_byte_array, 4);
    TEST_ASSERT_EQUAL_UINT8(byte_array[0], decoded_byte_array[0]);
    TEST_ASSERT_EQUAL_UINT8(byte_array[1], decoded_byte_array[1]);
}

void TestEepromReadWriteStatus(void) {
    if (!eeprom_test_enabled) {
        TEST_IGNORE_MESSAGE("Hardware test ignored");
    }
    byte status_register = 1 << 3;
    Eeprom::WriteStatusRegister(status_register);
    Eeprom::ReadStatusRegister(&status_register);

    TEST_ASSERT_EQUAL_UINT8(24, status_register);
}

void TestEepromReadWriteData(void) {
    if (!eeprom_test_enabled) {
        TEST_IGNORE_MESSAGE("Hardware test ignored");
    }
    byte write_buffer[345];
    uint16_t address = 1 << 10;
    for (uint32_t i = 0; i < 345; i++) {
        write_buffer[i] = (i * 41) % 71;
    }

    bool failed = false;
    try {
        Eeprom::WriteData((1 << 15), write_buffer, 345);
    } catch (etl::exception& e) {
        failed = true;
    }
    TEST_ASSERT_TRUE(failed);

    failed = false;
    try {
        Eeprom::WriteData(address, write_buffer, (1 << 15));
    } catch (etl::exception& e) {
        failed = true;
    }
    TEST_ASSERT_TRUE(failed);

    failed = false;
    try {
        Eeprom::WriteData((1 << 14), write_buffer, (1 << 14));
    } catch (etl::exception& e) {
        failed = true;
    }
    TEST_ASSERT_TRUE(failed);

    Eeprom::WriteData(address, write_buffer, 345);

    byte read_buffer[345] = {0};
    bool valid_buffer[345];
    uint16_t offset = 17;
    address += offset;

    failed = false;
    try {
        Eeprom::ReadData((1 << 15), read_buffer, 345 - offset, valid_buffer,
                         345 - offset);
    } catch (etl::exception& e) {
        failed = true;
    }
    TEST_ASSERT_TRUE(failed);

    failed = false;
    try {
        Eeprom::ReadData(address, read_buffer, (1 << 15), valid_buffer,
                         (1 << 15));
    } catch (etl::exception& e) {
        failed = true;
    }
    TEST_ASSERT_TRUE(failed);

    failed = false;
    try {
        Eeprom::ReadData((1 << 14), read_buffer, (1 << 14), valid_buffer,
                         (1 << 14));
    } catch (etl::exception& e) {
        failed = true;
    }
    TEST_ASSERT_TRUE(failed);

    try {
        Eeprom::ReadData(1, read_buffer, 2, valid_buffer, 1);
    } catch (etl::exception& e) {
        failed = true;
    }
    TEST_ASSERT_TRUE(failed);

    Eeprom::ReadData(address, read_buffer, 345 - offset, valid_buffer,
                     345 - offset);
    for (uint32_t i = 0; i < 345 - offset; i++) {
        TEST_ASSERT_EQUAL_UINT8(write_buffer[i + offset], read_buffer[i]);
        TEST_ASSERT_TRUE(valid_buffer[i]);
    }
}
