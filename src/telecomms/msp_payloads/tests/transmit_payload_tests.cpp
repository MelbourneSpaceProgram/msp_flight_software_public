#include <CppUTest/TestHarness.h>
#include <src/config/unit_tests.h>
#include <src/payload_processor/runnable_payload_processor.h>
#include <src/telecomms/fec/rs8.h>
#include <src/telecomms/msp_payloads/test_payload.h>
#include <src/telecomms/msp_payloads/transmit_packet.h>

TEST_GROUP(TransmitPayload){};

TEST(TransmitPayload, DecodeTransmitPacket) {
    TestPayload test_payload;
    TransmitPacket test_packet(&test_payload);
    byte serialised_test_packet[Rs8::kBlockLength];
    test_packet.SerialiseTo(serialised_test_packet);

    // Flip a bit to see if it gets corrected
    serialised_test_packet[4] ^= 1UL << 6;

    byte decode_block[RunnablePayloadProcessor::kDecodeBlockLength] = {0};

    // Copy the uplink data and padding into the decode block,
    // such that it is prefixed with 0 filled padding.
    memcpy(decode_block, serialised_test_packet,
           Rs8::kBlockLength * sizeof(byte));

    int16_t msp_packet_length;
    byte* msp_packet;

    uint8_t errors_corrected = Rs8::Decode(decode_block, NULL, 0);
    // Confirm only one error was corrected
    CHECK_EQUAL(1, errors_corrected);
    msp_packet = &decode_block[RunnablePayloadProcessor::kDecodeDataIndex];
    msp_packet_length = msp_packet[RunnablePayloadProcessor::kMspLengthIndex];

    uint8_t test_payload_data_offset =
        0x04;  // tx count, rx count, command success
               // count, payload code (1 byte each)

    // Last byte of test payload is a carriage return which is not included in
    // the test message.
    for (uint8_t i = 0; i < msp_packet_length; i++) {
        if (i < TestPayload::kTestMessageSize - 1) {
            CHECK_EQUAL(TestPayload::kTestMessage[i],
                        msp_packet[i + test_payload_data_offset]);
        } else if (i == TestPayload::kTestMessageSize - 1) {
            CHECK_EQUAL(0x0d, msp_packet[i + test_payload_data_offset]);
        } else {
            CHECK_EQUAL(0x00, msp_packet[i + test_payload_data_offset]);
        }
    }
}
