#include <CppUTest/TestHarness.h>
#include <src/util/hmac.h>
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
    // input1: signature[4] + length[1] + sequence[2], data[6] = MspPacket[13]
    // input2: hash key
    byte msp_packet[13] = {
             0x5c, 0x90, 0xca, 0xe3, // HMAC checksum
             13, // Length
             0x01, 0x00, // Sequence
             0x03, 0x00, 0x08, 0x00, 0x00, 0x00 // Data
    };

    // Length of the MSP packet, including the signature.
    int16_t msp_packet_length = RunnablePayloadProcessor::GetMspPacketLength(msp_packet, 13);
    CHECK(msp_packet_length >= 0);
    CHECK(msp_packet_length == 13);

    // Ensure HMAC matches.
    CHECK(RunnablePayloadProcessor::CheckHmac(msp_packet, msp_packet_length));
}

// TODO(crozone): Rewrite other HMAC tests based on the above test.
//
// TODO(crozone): Test case 2 where the signature should equal the hmac output
// TODO(crozone): Test case 3 where the signature should equal the hmac output
// TODO(crozone): Test for a case where the signature should not equal the hmac output
