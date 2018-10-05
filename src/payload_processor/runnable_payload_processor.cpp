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
        memset(decode_block, 0, 255 * sizeof(byte));

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
        std::string msp_signature(reinterpret_cast<char *>(&packet_body),
                                  kMspSignatureBytes);

        int16_t msp_packet_len =
            static_cast<int16_t>(packet_body[kMspSignatureBytes]) -
            static_cast<int16_t>(kMspSignatureBytes);

        Log_info1("Reported packet length: %d", msp_packet_len);

        if (msp_packet_len < 0) {
            Log_error2("Payload length too short! (%d bytes < %d)",
                       msp_packet_len, 0);
            continue;
        } else if (msp_packet_len > 96) {
            Log_error2("Payload length too long! (%d bytes > %d)",
                       msp_packet_len, 96);
            continue;
        }

        byte *msp_packet = &packet_body[kMspSignatureBytes];
        std::string hash_key = kHmacKey;

        // Produce the sha1-hmac hash and truncate down to the the first 4
        // bytes (8 chars)
        std::string hash =
            hmac<SHA1>(msp_packet, static_cast<size_t>(msp_packet_len),
                       &hash_key[0], hash_key.size())
                .substr(0, kNumBytesTruncated * kNumCharsPerByte);

        // Convert hash chars into hex form (i.e. 0x__0x__..)
        std::string hex_hash = StringHex::StringToHex(hash);

        // Verify that the hash matches the signature
        if (msp_signature.compare(hex_hash) == 0) {
            byte *msp_payload = &packet_body[kMspSignatureBytes + kLengthBytes +
                                             kSequenceNumberBytes];
            payload_processor.ParseAndExecuteCommands(msp_payload);
        } else {
            // TODO(danieL632): c++ style static_cast<xdc_IArg> not working
            Log_error2("Signature does not match hash: sig = %s - hash = %s",
                       (xdc_IArg)msp_signature.c_str(), (xdc_IArg)hash.c_str());
        }
    }
}
