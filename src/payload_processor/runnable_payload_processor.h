#ifndef SRC_PAYLOAD_PROCESSOR_RUNNABLE_PAYLOAD_PROCESSOR_H_
#define SRC_PAYLOAD_PROCESSOR_RUNNABLE_PAYLOAD_PROCESSOR_H_

#include <src/tasks/runnable.h>

class RunnablePayloadProcessor : public Runnable {
   public:
    fnptr GetRunnablePointer();
    RunnablePayloadProcessor();

   private:
    static void ExecuteCommandsInLithiumPayload();
    static const uint8_t kAx25Bytes = 16;
    static const uint8_t kSignatureBytes = 4;
    static const uint8_t kLengthBytes = 1;
    static const uint8_t kSequenceNumberBytes = 2;
};

#endif  //  SRC_PAYLOAD_PROCESSOR_RUNNABLE_PAYLOAD_PROCESSOR_H_
