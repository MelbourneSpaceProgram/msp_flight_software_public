#ifndef SRC_TELECOMMS_RUNNABLE_LITHIUM_H_
#define SRC_TELECOMMS_RUNNABLE_LITHIUM_H_

#include <src/tasks/runnable.h>

class RunnableLithium : public Runnable {
   public:
    RunnableLithium();
    fnptr GetRunnablePointer();

   private:
    static void LithiumStream();
};

#endif  // SRC_TELECOMMS_RUNNABLE_LITHIUM_H_
