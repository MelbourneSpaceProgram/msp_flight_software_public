#ifndef SRC_TELECOMMS_RUNNABLE_LITHIUM_LISTENER_H_
#define SRC_TELECOMMS_RUNNABLE_LITHIUM_LISTENER_H_

class RunnableLithiumListener : public Runnable {
   public:
    fnptr GetRunnablePointer();
    RunnableLithiumListener();

   private:
    static void Receive();
    // ENSURE THAT THE LITHIUM UART HANDLE HAS A TIMEOUT SET
    static bool ReadLithiumUart(byte* read_buffer, uint8_t size);
};

#endif  // SRC_TELECOMMS_RUNNABLE_LITHIUM_LISTENER_H_
