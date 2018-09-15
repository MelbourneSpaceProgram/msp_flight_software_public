#include <CppUTest/TestHarness.h>
#include <external/hmac/hmac.h>
#include <external/hmac/sha1.h>
#include <src/config/unit_tests.h>
#include <src/payload_processor/runnable_payload_processor.h>
#include <src/util/data_types.h>
#include <src/util/string_hex.h>

/**
 * Tests sha1hmac which is being implemented in runnable_payload_processor.cpp
 *
 * Resource:
 * The following link provides an online sha1-hmac implementation
 * https://www.liavaag.org/English/SHA-Generator/HMAC/
 * Set the input and output types to HEX, and set the key type to String (For
 * a key like "SuperSecretKey").
 */

TEST_GROUP(Sha1Hmac){};

// Test case 1 where the signature should equal the hmac output
TEST(Sha1Hmac, TestSha1HmacValid1) {
    // the packet signature (i.e. the expected output)
    std::string msp_signature("\x5c\x90\xca\xe3");

    // input1: length byte(1), sequence bytes(2), payload bytes(remaining)
    // input2: hash key
    byte msp_packet[] = {0x0d, 0x01, 0x00, 0x03, 0x00, 0x08, 0x00, 0x00, 0x00};
    uint8_t msp_packet_len = (static_cast<uint8_t>(msp_packet[0])) -
                             RunnablePayloadProcessor::kMspSignatureBytes;
    std::string hash_key = "SuperSecretKey";

    // output - keeping only the first 4 bytes (8 chars)
    std::string hash =
        hmac<SHA1>(&msp_packet, msp_packet_len, &hash_key[0], hash_key.size());
    std::string hash_truncated = hash.substr(0, 8);
    std::string hex_hash = StringHex::StringToHex(hash_truncated);

    // STATUS: Not Working!
    CHECK(hex_hash.compare(msp_signature) == 0);
}

// Test case 2 where the signature should equal the hmac output
TEST(Sha1Hmac, TestSha1HmacValid2) {
    // the packet signature (i.e. the expected output)
    std::string msp_signature("\xa0\x0e\x9a\x62");

    // input1: length byte(1), sequence bytes(2), payload bytes(remaining)
    // input2: hash key
    byte msp_packet[] = {
        0x53, 0x01, 0x00, 0xff, 0x00, 0x09, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0xf0, 0x3f, 0x11, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x40, 0x19,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x08, 0x40, 0x21, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x10, 0x40, 0x29, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x14, 0x40, 0x31, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x18, 0x40, 0x39,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x1c, 0x40, 0x41, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x20, 0x40, 0x00, 0x00};
    uint8_t msp_packet_len = (static_cast<uint8_t>(msp_packet[0])) -
                             RunnablePayloadProcessor::kMspSignatureBytes;
    std::string hash_key = "SuperSecretKey";

    // output - keeping only the first 4 bytes (8 chars)
    std::string hash =
        hmac<SHA1>(&msp_packet, msp_packet_len, &hash_key[0], hash_key.size());
    std::string hash_truncated = hash.substr(0, 8);
    std::string hex_hash = StringHex::StringToHex(hash_truncated);

    CHECK(hex_hash.compare(msp_signature) == 0);
}

// Test case 3 where the signature should equal the hmac output
TEST(Sha1Hmac, TestSha1HmacValid3) {
    // the packet signature (i.e. the expected output)
    std::string msp_signature("\xf2\x56\xe5\x52");

    // input1: length byte(1), sequence bytes(2), payload bytes(remaining)
    // input2: hash key
    byte msp_packet[] = {0x27, 0x03, 0x00, 0xff, 0x00, 0x0d, 0x0d, 0x00, 0x00,
                         0x00, 0x11, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                         0x00, 0x19, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                         0x00, 0x25, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00};
    uint8_t msp_packet_len = (static_cast<uint8_t>(msp_packet[0])) -
                             RunnablePayloadProcessor::kMspSignatureBytes;
    std::string hash_key = "SuperSecretKey";

    // output - keeping only the first 4 bytes (8 chars)
    std::string hash =
        hmac<SHA1>(&msp_packet, msp_packet_len, &hash_key[0], hash_key.size());
    std::string hash_truncated = hash.substr(0, 8);
    std::string hex_hash = StringHex::StringToHex(hash_truncated);

    CHECK(hex_hash.compare(msp_signature) == 0);
}

// Test for a case where the signature should not equal the hmac output
TEST(Sha1Hmac, TestSha1HmacInvalid) {
    // the packet signature (i.e. the expected output)
    std::string msp_signature(
        "\x5c\x90\xca\xe4");  // last byte should be e3, not e4

    // input1: length byte(1), sequence bytes(2), payload bytes(remaining)
    // input2: hash key
    byte msp_packet[] = {0x0d, 0x01, 0x00, 0x03, 0x00, 0x08, 0x00, 0x00, 0x00};
    uint8_t msp_packet_len = (static_cast<uint8_t>(msp_packet[0])) -
                             RunnablePayloadProcessor::kMspSignatureBytes;
    std::string hash_key = "SuperSecretKey";

    // output - keeping only the first 4 bytes (8 chars)
    std::string hash =
        hmac<SHA1>(&msp_packet, msp_packet_len, &hash_key[0], hash_key.size());
    std::string hash_truncated = hash.substr(0, 8);
    std::string hex_hash = StringHex::StringToHex(hash_truncated);

    CHECK(hex_hash.compare(msp_signature) != 0);
}
