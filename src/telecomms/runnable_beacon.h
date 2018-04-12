#ifndef SRC_TELECOMMS_RUNNABLE_BEACON_H_
#define SRC_TELECOMMS_RUNNABLE_BEACON_H_

#include <src/tasks/runnable.h>

class RunnableBeacon : public Runnable {
   public:
    RunnableBeacon();
    fnptr GetRunnablePointer();

   private:
    static void Beacon();
};

#endif  // SRC_TELECOMMS_RUNNABLE_BEACON_H_
