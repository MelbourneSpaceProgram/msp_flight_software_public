#ifndef SRC_PAYLOAD_PROCESSOR_RUNNABLE_PAYLOAD_PROCESSOR_H_
#define SRC_PAYLOAD_PROCESSOR_RUNNABLE_PAYLOAD_PROCESSOR_H_

#include <src/tasks/runnable.h>
#include <src/util/data_types.h>

class RunnablePayloadProcessor : public Runnable {
   public:
    fnptr GetRunnablePointer();
    RunnablePayloadProcessor();

    // Constants fundamental to both uplink packet and FEC
    static const uint8_t kDataLength = 96;
    static const uint8_t kParityLength = 32;

    // Constants relating to the uplink payload
    //   UplinkPayload
    // = Ax25[16] + Data[96] + Parity[32]
    static const uint8_t kUplinkAx25Index = 0;
    static const uint8_t kUplinkAx25Length = 16;
    static const uint8_t kUplinkDataIndex =
        kUplinkAx25Index + kUplinkAx25Length;
    static const uint8_t kUplinkParityIndex = kUplinkDataIndex + kDataLength;
    static const uint8_t kUplinkLength =
        kUplinkAx25Length + kDataLength + kParityLength;

    // Constants relating to RS FEC Decoding
    //   DecodeBlock[255]
    // = ZeroPadding[127] + Data[96] + Parity[32]
    static const uint8_t kDecodePaddingIndex = 0;
    static const uint8_t kDecodePaddingLength = 127;
    static const uint8_t kDecodeDataIndex =
        kDecodePaddingIndex + kDecodePaddingLength;
    static const uint8_t kDecodeParityIndex = kDecodeDataIndex + kDataLength;
    static const uint8_t kDecodeBlockLength =
        kDecodePaddingLength + kDataLength + kParityLength;

    // Constants relating to the MSP packet
    //   MspPacket[7-96]
    // = Header[7] + Command[0-89]
    // = MspSignature[4] + Length[1] + SequenceNumber[2] + Command[0-89]
    static const uint8_t kMspSignatureIndex = 0;
    static const uint8_t kMspSignatureLength = 4;
    static const uint8_t kMspLengthIndex =
        kMspSignatureIndex + kMspSignatureLength;
    static const uint8_t kMspLengthLength = 1;
    static const uint8_t kMspSequenceNumberIndex =
        kMspLengthIndex + kMspLengthLength;
    static const uint8_t kMspSequenceNumberLength = 2;
    static const uint8_t kMspHeaderIndex = kMspSignatureIndex;
    static const uint8_t kMspHeaderLength =
        kMspSignatureLength + kMspLengthLength + kMspSequenceNumberLength;
    static const uint8_t kMspCommandIndex = kMspHeaderIndex + kMspHeaderLength;
    static const uint8_t kMspCommandMaxLength = kDataLength - kMspHeaderLength;

    static bool ProcessPayload(byte command[], const byte uplink_payload[]);
    static bool DecodeFec(byte decode_block[]);
    static int16_t GetMspPacketLength(const byte msp_packet[],
                                      int16_t max_length);
    static bool CheckHmac(const byte msp_packet[], int16_t msp_packet_length);

   private:
    static void ExecuteCommandsInLithiumPayload();
};

#endif  //  SRC_PAYLOAD_PROCESSOR_RUNNABLE_PAYLOAD_PROCESSOR_H_
