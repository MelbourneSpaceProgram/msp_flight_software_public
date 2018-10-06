#ifndef SRC_TELECOMMS_RUNNABLE_BEACON_H_
#define SRC_TELECOMMS_RUNNABLE_BEACON_H_

#include <src/tasks/runnable.h>

class LithiumBeaconPeriodCommand;

class RunnableBeacon : public Runnable {
    friend class LithiumBeaconPeriodCommand;

   public:
    RunnableBeacon();
    static uint32_t GetBeaconPeriodMs();
    fnptr GetRunnablePointer();

   private:
    static void Beacon(uintptr_t arg1, uintptr_t arg2);
    static uint32_t beacon_period_ms;
};

#endif  // SRC_TELECOMMS_RUNNABLE_BEACON_H_
