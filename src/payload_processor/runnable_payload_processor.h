#ifndef SRC_PAYLOAD_PROCESSOR_RUNNABLE_PAYLOAD_PROCESSOR_H_
#define SRC_PAYLOAD_PROCESSOR_RUNNABLE_PAYLOAD_PROCESSOR_H_

#include <src/tasks/runnable.h>
#include <src/util/data_types.h>

class RunnablePayloadProcessor : public Runnable {
    friend class TEST_PayloadProcessor_TestSequence_Test;

   public:
    fnptr GetRunnablePointer();
    RunnablePayloadProcessor();

    // Constants fundamental to both uplink packet and FEC
    static constexpr uint8_t kDataLength = 96;
    static constexpr uint8_t kParityLength = 32;

    // Constants relating to the uplink payload
    //   Command
    // = Ax25[16] + Data[96] + Parity[32]
    static constexpr uint8_t kUplinkAx25Index = 0;
    static constexpr uint8_t kUplinkAx25Length = 16;
    static constexpr uint8_t kUplinkDataIndex =
        kUplinkAx25Index + kUplinkAx25Length;
    static constexpr uint8_t kUplinkParityIndex =
        kUplinkDataIndex + kDataLength;
    static constexpr uint8_t kUplinkLength =
        kUplinkAx25Length + kDataLength + kParityLength;

    // Constants relating to RS FEC Decoding
    //   DecodeBlock[255]
    // = ZeroPadding[127] + Data[96] + Parity[32]
    static constexpr uint8_t kDecodePaddingIndex = 0;
    static constexpr uint8_t kDecodePaddingLength = 127;
    static constexpr uint8_t kDecodeDataIndex =
        kDecodePaddingIndex + kDecodePaddingLength;
    static constexpr uint8_t kDecodeParityIndex =
        kDecodeDataIndex + kDataLength;
    static constexpr uint8_t kDecodeBlockLength =
        kDecodePaddingLength + kDataLength + kParityLength;

    // Constants relating to the MSP packet
    //   MspPacket[7-96]
    // = Header[7] + Uplink[0-89]
    // = MspSignature[4] + Length[1] + SequenceNumber[2] + Uplink[0-89]
    static constexpr uint8_t kMspSignatureIndex = 0;
    static constexpr uint8_t kMspSignatureLength = 4;
    static constexpr uint8_t kMspLengthIndex =
        kMspSignatureIndex + kMspSignatureLength;
    static constexpr uint8_t kMspLengthLength = 1;
    static constexpr uint8_t kMspSequenceNumberIndex =
        kMspLengthIndex + kMspLengthLength;
    static constexpr uint8_t kMspSequenceNumberLength = 2;
    static constexpr uint8_t kMspHeaderIndex = kMspSignatureIndex;
    static constexpr uint8_t kMspHeaderLength =
        kMspSignatureLength + kMspLengthLength + kMspSequenceNumberLength;
    static constexpr uint8_t kMspUplinkIndex =
        kMspHeaderIndex + kMspHeaderLength;
    static constexpr uint8_t kMspUplinkMaxLength =
        kDataLength - kMspHeaderLength;

    static bool ProcessPayload(byte command[], const byte uplink_payload[]);
    static bool DecodeFec(byte decode_block[]);
    static int16_t GetMspPacketLength(const byte msp_packet[],
                                      int16_t max_length);
    static bool CheckHmac(const byte msp_packet[], int16_t msp_packet_length);
    static bool CheckSequence(const byte msp_packet[]);
    static uint16_t GetSequence();

   private:
    static uint16_t sequence;
    static void ExecuteUplinksInLithiumPayload();

    static bool use_fec;
    static bool check_hmac;
    static bool check_sequence;
};

#endif  //  SRC_PAYLOAD_PROCESSOR_RUNNABLE_PAYLOAD_PROCESSOR_H_
