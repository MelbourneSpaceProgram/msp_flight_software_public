#include <external/hmac/sha1.h>
#include <src/config/satellite.h>
#include <src/payload_processor/payload_processor.h>
#include <src/payload_processor/runnable_payload_processor.h>
#include <src/tasks/runnable.h>
#include <src/telecomms/fec/rs8.h>
#include <src/telecomms/lithium.h>
#include <src/util/hmac.h>
#include <src/util/string_hex.h>
#include <stdint.h>
#include <ti/sysbios/BIOS.h>
#include <ti/sysbios/knl/Mailbox.h>
#include <xdc/runtime/Log.h>

// TODO(daniel632): A proper key needs to be decided on.
static const std::string kHmacKey = "SuperSecretKey";
uint16_t RunnablePayloadProcessor::sequence = 0;
bool RunnablePayloadProcessor::use_fec = kUseFecDefault;
bool RunnablePayloadProcessor::check_hmac = kCheckHmacDefault;
bool RunnablePayloadProcessor::check_sequence = kCheckSequenceDefault;

RunnablePayloadProcessor::RunnablePayloadProcessor() {}

fnptr RunnablePayloadProcessor::GetRunnablePointer() {
    return &RunnablePayloadProcessor::ExecuteUplinksInLithiumPayload;
}

void RunnablePayloadProcessor::ExecuteUplinksInLithiumPayload() {
    PayloadProcessor payload_processor;
    byte lithium_payload[Lithium::kMaxReceivedUplinkSize];

    Mailbox_Handle payload_mailbox_handle =
        Lithium::GetInstance()->GetUplinkMailbox();

    while (1) {
        Mailbox_pend(payload_mailbox_handle, &lithium_payload,
                     BIOS_WAIT_FOREVER);

        byte command[kMspUplinkMaxLength] = {0};
        if (ProcessPayload(command, lithium_payload)) {
            payload_processor.ParseAndExecuteUplinks(command);
        } else {
            // Payload failed on FEC or HMAC
            continue;
        }
    }
}

/// Process a lithium payload and produce an output command.
bool RunnablePayloadProcessor::ProcessPayload(byte command[],
                                              const byte uplink_payload[]) {
    byte decode_block[kDecodeBlockLength] = {0};

    // Copy the uplink data and padding into the decode block,
    // such that it is prefixed with 0 filled padding.
    memcpy(&decode_block[kDecodeDataIndex], &uplink_payload[kUplinkDataIndex],
           (kDataLength + kParityLength) * sizeof(byte));

    int16_t msp_packet_length;
    byte* msp_packet;
    if (use_fec) {
        if (!DecodeFec(decode_block)) return false;
        msp_packet = &decode_block[kDecodeDataIndex];
    } else {
        // TODO(dingbenjamin): Find a better way to do this with the memcpy
        // removed and without constness removal
        byte msp_packet_buffer[kDataLength];
        msp_packet = msp_packet_buffer;
        memcpy(msp_packet, &uplink_payload[kUplinkDataIndex],
               kDataLength * sizeof(byte));
    }

    // Length of the MSP packet, including the signature.
    msp_packet_length = GetMspPacketLength(msp_packet, kDataLength);
    if (msp_packet_length < 0) return false;

    if (check_hmac && !CheckHmac(msp_packet, msp_packet_length)) return false;

    if (check_sequence && !CheckSequence(msp_packet)) return false;

    // Copy the command to the command buffer
    memcpy(command, &msp_packet[kMspUplinkIndex],
           (msp_packet_length - kMspHeaderLength) * sizeof(byte));

    return true;
}

bool RunnablePayloadProcessor::DecodeFec(byte decode_block[]) {
    // Decode using Reed-Solomon (255, 223) CCSDS
    int32_t errors_corrected = Rs8::Decode(decode_block, NULL, 0);

    // Check if packet was recovered.
    // Decode returns the number of errors corrected only if the packet
    // was successfully decoded.
    // If the packet couldn't be decoded (because there were more than 16
    // errors), Decode returns -1.
    if (errors_corrected < 0) {
        Log_error0("RS8 Decode failed. Too many corrupted bytes.");
        return false;
    }

    return true;
}

int16_t RunnablePayloadProcessor::GetMspPacketLength(const byte msp_packet[],
                                                     int16_t max_length) {
    int16_t msp_packet_length = msp_packet[kMspLengthIndex];

    if (msp_packet_length <= kMspHeaderLength) {
        Log_error2("MSP packet length too short! (%d bytes <= %d)",
                   msp_packet_length, kMspHeaderLength);
        return -1;
    } else if (msp_packet_length > max_length) {
        Log_error2("MSP packet length too long! (%d bytes > %d)",
                   msp_packet_length, max_length);
        return -1;
    }

    return msp_packet_length;
}

bool RunnablePayloadProcessor::CheckHmac(const byte msp_packet[],
                                         int16_t msp_packet_length) {
    std::string hash_key = kHmacKey;

    // Produce the sha1-hmac hash of the length byte onwards,
    // up until (msp_packet_length - kMspSignatureLength) bytes.
    unsigned char calculated_hash[SHA1::HashBytes];
    hmac<SHA1>(calculated_hash, &msp_packet[kMspLengthIndex],
               msp_packet_length - kMspSignatureLength,
               reinterpret_cast<const byte*>(hash_key.data()), hash_key.size());

    // Verify that the hash matches the signature
    if (memcmp(msp_packet, calculated_hash, kMspSignatureLength) == 0) {
        return true;
    } else {
        // TODO(crozone): Print signature difference
        Log_error0("Signature does not match hash");
        // Log_error2("Signature does not match hash: sig = %s - hash = %s",
        //           (xdc_IArg)msp_signature.c_str(), (xdc_IArg)hash.c_str());
        return false;
    }
}

bool RunnablePayloadProcessor::CheckSequence(const byte msp_packet[]) {
    uint16_t received_sequence =
        (static_cast<uint16_t>(msp_packet[kMspSequenceNumberIndex]) << 8) +
        static_cast<uint16_t>(msp_packet[kMspSequenceNumberIndex + 1]);
    if (received_sequence != sequence) {
        // TODO(dingbenjamin): Mute logs in unit test
#if not defined TEST_CONFIGURATION
        Log_error2("Received sequence %d does not match current sequence %d",
                   received_sequence, sequence);
#endif
        return false;
    }
    sequence = sequence == UINT16_MAX ? 0 : sequence + 1;
    return true;
}

uint16_t RunnablePayloadProcessor::GetSequence() { return sequence; }
