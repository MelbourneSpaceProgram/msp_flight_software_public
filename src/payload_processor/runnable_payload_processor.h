#ifndef SRC_PAYLOAD_PROCESSOR_RUNNABLE_PAYLOAD_PROCESSOR_H_
#define SRC_PAYLOAD_PROCESSOR_RUNNABLE_PAYLOAD_PROCESSOR_H_

#include <src/tasks/runnable.h>

class RunnablePayloadProcessor : public Runnable {
   public:
    fnptr GetRunnablePointer();
    RunnablePayloadProcessor();

   private:
    static void ExecuteCommandsInLithiumPayload();
    static const uint8_t kMspHeaderBytes = 4;
    static const uint8_t kSequenceSecurityBytes = 2;
};

#endif  //  SRC_PAYLOAD_PROCESSOR_RUNNABLE_PAYLOAD_PROCESSOR_H_
