#include <external/hmac/hmac.h>
#include <external/hmac/sha1.h>
#include <src/payload_processor/payload_processor.h>
#include <src/payload_processor/runnable_payload_processor.h>
#include <src/tasks/runnable.h>
#include <src/telecomms/fec/rs8.h>
#include <src/telecomms/lithium.h>
#include <src/util/string_hex.h>
#include <ti/sysbios/BIOS.h>
#include <ti/sysbios/knl/Mailbox.h>
#include <xdc/runtime/Log.h>

// TODO(daniel632): A proper key needs to be decided on.
static const std::string kHmacKey = "SuperSecretKey";

RunnablePayloadProcessor::RunnablePayloadProcessor() {}

fnptr RunnablePayloadProcessor::GetRunnablePointer() {
    return &RunnablePayloadProcessor::ExecuteCommandsInLithiumPayload;
}

void RunnablePayloadProcessor::ExecuteCommandsInLithiumPayload() {
    PayloadProcessor payload_processor;
    byte lithium_payload[Lithium::kMaxReceivedUplinkSize];
    Mailbox_Handle payload_mailbox_handle =
        Lithium::GetInstance()->GetUplinkMailbox();

    while (1) {
        Mailbox_pend(payload_mailbox_handle, &lithium_payload,
                     BIOS_WAIT_FOREVER);

        // Decode using Reed-Solomon FEC
        //
        // Packet body = lithium_payload + kAx25Bytes
        // Decode block length = 255
        // Decode padding length = 127
        // Decode data length = 96
        // Decode parity length = 32

        // On upload we pad the start with 128 bytes
        // TODO(crozone): Make 127 and 255 a constant somewhere
        byte decode_block[255];
        memset(decode_block, 0, sizeof(decode_block));

        // Copy the MSP body into the payload such that it is prefixed with 127
        // bytes of padding
        memcpy(&decode_block[127], &lithium_payload[kAx25Bytes],
               128 * sizeof(byte));

        // Decode using Reed-Solomon (255, 223) CCSDS
        int32_t errors_corrected = Rs8::Decode(decode_block, NULL, 0);

        // Check if packet was recovered. If there were more than 16 errors, it
        // will have failed.
        if (errors_corrected < 0) {
            Log_error0("RS8 Decode failed. Too many corrupted bytes.");
            continue;
        }

        Log_info1("Payload bytes corrected: %d", errors_corrected);

        byte *packet_body = &decode_block[127];

        // Verifying that the payload is legitimate (i.e. authenticating it
        // with sha1-hmac), by hashing over the length, sequence, and message.

        int16_t msp_packet_len =
            static_cast<int16_t>(packet_body[kMspSignatureBytes]) -
            static_cast<int16_t>(kMspSignatureBytes);

        Log_info1("Reported packet length: %d", msp_packet_len);

        if (msp_packet_len < 0) {
            Log_error2("Payload length too short! (%d bytes < %d)",
                       msp_packet_len, 0);
            continue;
        } else if (msp_packet_len > (96 - kMspSignatureBytes)) {
            Log_error2("Payload length too long! (%d bytes > %d)",
                       msp_packet_len, (96 - kMspSignatureBytes));
            continue;
        }

        // This is the signature embedded in the payload, of length 4.
        byte* msp_signature = &packet_body[0];

        // This is the payload of the packet, after the signature.
        byte *msp_packet = &packet_body[kMspSignatureBytes];
        std::string hash_key = kHmacKey;

        // Produce the sha1-hmac hash
        std::string hash =
            hmac<SHA1>(msp_packet, static_cast<size_t>(msp_packet_len),
                       &hash_key[0], hash_key.size());

        byte calculated_short_hash[kMspSignatureBytes];
        memset(calculated_short_hash, 0, sizeof(calculated_short_hash));

        // hash is a hex encoded string
        // Convert the first 8 characters of hash from hex string
        // representation into a raw byte array (4 bytes).
        for(int i = 0; i < kMspSignatureBytes; i++) {
            // Convert upper/leftmost character to upper nibble of byte
            char hash_char_upper = hash.at(i * 2);
            if(hash_char_upper >= '0' && hash_char_upper <= '9') {
                calculated_short_hash[i] |= (hash_char_upper - '0') << 4;
            }
            else if(hash_char_upper >= 'A' && hash_char_upper <= 'F') {
                calculated_short_hash[i] |= (hash_char_upper - 'A' + 10) << 4;
            }
            else if(hash_char_upper >= 'a' && hash_char_upper <= 'f') {
                calculated_short_hash[i] |= (hash_char_upper - 'a' + 10) << 4;
            }

            // Convert lower/rightmost character to lower nibble of byte
            char hash_char_lower = hash.at((i * 2) + 1);
            if(hash_char_lower >= '0' && hash_char_lower <= '9') {
                calculated_short_hash[i] |= (hash_char_lower - '0') & 0x0F;
            }
            else if(hash_char_lower >= 'A' && hash_char_lower <= 'F') {
                calculated_short_hash[i] |= (hash_char_lower - 'A' + 10) & 0x0F;
            }
            else if(hash_char_lower >= 'a' && hash_char_lower <= 'f') {
                calculated_short_hash[i] |= (hash_char_lower - 'a' + 10) & 0x0F;
            }
        }

        // Verify that the hash matches the signature
        if (memcmp(msp_signature, calculated_short_hash, kMspSignatureBytes) == 0) {
            byte *msp_payload = &packet_body[kMspSignatureBytes + kLengthBytes +
                                             kSequenceNumberBytes];
            payload_processor.ParseAndExecuteCommands(msp_payload);
        } else {
            Log_error2("Signature does not match hash: sig = %02x%02x%02x%02x - hash = %02x%02x%02x%02x",
                       msp_signature[0], msp_signature[1], msp_signature[2], msp_signature[3],
                       calculated_short_hash[0], calculated_short_hash[1], calculated_short_hash[2], calculated_short_hash[3]);
        }
    }
}
