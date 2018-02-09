#ifndef SRC_TELECOMMS_RUNNABLE_LITHIUM_RECEIVER_H_
#define SRC_TELECOMMS_RUNNABLE_LITHIUM_RECEIVER_H_

class RunnableLithiumListener : public Runnable {
   public:
    fnptr GetRunnablePointer();
    RunnableLithiumListener();

   private:
    static void Receive();
};

#endif  // SRC_TELECOMMS_RUNNABLE_LITHIUM_RECEIVER_H_
