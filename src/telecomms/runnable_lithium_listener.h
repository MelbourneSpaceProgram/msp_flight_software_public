#ifndef SRC_TELECOMMS_RUNNABLE_LITHIUM_LISTENER_H_
#define SRC_TELECOMMS_RUNNABLE_LITHIUM_LISTENER_H_

class RunnableLithiumListener : public Runnable {
   public:
    fnptr GetRunnablePointer();
    RunnableLithiumListener();

   private:
    static void Receive();
    static const byte kReceivedDataCode =
        0x04;  // TODO(dingbenjamin): This may not be the best location
};

#endif  // SRC_TELECOMMS_RUNNABLE_LITHIUM_LISTENER_H_
