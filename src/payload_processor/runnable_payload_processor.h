#ifndef SRC_PAYLOAD_PROCESSOR_RUNNABLE_PAYLOAD_PROCESSOR_H_
#define SRC_PAYLOAD_PROCESSOR_RUNNABLE_PAYLOAD_PROCESSOR_H_

class RunnablePayloadProcessor : public Runnable {
   public:
    fnptr GetRunnablePointer();
    RunnablePayloadProcessor();

   private:
    static void ExecuteCommandsInLithiumPayload();
};

#endif  //  SRC_PAYLOAD_PROCESSOR_RUNNABLE_PAYLOAD_PROCESSOR_H_
