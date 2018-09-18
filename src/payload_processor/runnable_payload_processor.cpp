#include <external/hmac/hmac.h>
#include <external/hmac/sha1.h>
#include <src/payload_processor/payload_processor.h>
#include <src/payload_processor/runnable_payload_processor.h>
#include <src/tasks/runnable.h>
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

        // Verifying that the payload is legitimate (i.e. authenticating it
        // with sha1-hmac), by hashing over the length, sequence, and message.
        std::string msp_signature(
            reinterpret_cast<char *>(&lithium_payload[kAx25Bytes]),
            kMspSignatureBytes);

        uint8_t msp_packet_len =
            lithium_payload[kAx25Bytes + kMspSignatureBytes] -
            kMspSignatureBytes;

        byte *msp_packet = &lithium_payload[kAx25Bytes + kMspSignatureBytes];
        std::string hash_key = kHmacKey;

        // Produce the sha1-hmac hash and truncate down to the the first 4
        // bytes (8 chars)
        std::string hash =
            hmac<SHA1>(msp_packet, msp_packet_len, &hash_key[0],
                       hash_key.size())
                .substr(0, kNumBytesTruncated * kNumCharsPerByte);

        // Convert hash chars into hex form (i.e. 0x__0x__..)
        std::string hex_hash = StringHex::StringToHex(hash);

        // Verify that the hash matches the signature
        if (msp_signature.compare(hex_hash) == 0) {
            byte *msp_payload = lithium_payload + kAx25Bytes +
                                kMspSignatureBytes + kLengthBytes +
                                kSequenceNumberBytes;
            payload_processor.ParseAndExecuteCommands(msp_payload);
        } else {
            // TODO(danieL632): c++ style static_cast<xdc_IArg> not working
            Log_error2("Signature does not match hash: sig = %s - hash = %s",
                       (xdc_IArg)msp_signature.c_str(), (xdc_IArg)hash.c_str());
        }
    }
}
