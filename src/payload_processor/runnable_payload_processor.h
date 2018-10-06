#ifndef SRC_PAYLOAD_PROCESSOR_RUNNABLE_PAYLOAD_PROCESSOR_H_
#define SRC_PAYLOAD_PROCESSOR_RUNNABLE_PAYLOAD_PROCESSOR_H_

#include <src/tasks/runnable.h>

class RunnablePayloadProcessor : public Runnable {
   public:
    fnptr GetRunnablePointer();
    RunnablePayloadProcessor();
    static const uint8_t kAx25Bytes = 16;
    static const uint8_t kMspSignatureBytes = 4;
    static const uint8_t kLengthBytes = 1;
    static const uint8_t kSequenceNumberBytes = 2;
    static const uint8_t kNumBytesTruncated = 4;
    static const uint8_t kNumCharsPerByte = 2;  // (byte in hex representation)

   private:
    static void ExecuteCommandsInLithiumPayload(uintptr_t arg1, uintptr_t arg2);
};

#endif  //  SRC_PAYLOAD_PROCESSOR_RUNNABLE_PAYLOAD_PROCESSOR_H_
