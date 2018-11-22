#ifndef SRC_TELECOMMS_RUNNABLE_BEACON_H_
#define SRC_TELECOMMS_RUNNABLE_BEACON_H_

#include <src/tasks/runnable.h>
#include <src/util/data_types.h>

class LithiumBeaconPeriodCommand;

class RunnableBeacon : public Runnable {
    friend class LithiumBeaconPeriodUplink;

   public:
    explicit RunnableBeacon(bool limp_mode = false);
    static uint32_t GetBeaconPeriodMs();
    fnptr GetRunnablePointer();

   private:
    static void Beacon();
    static bool limp_mode_beacon;
    static bool initialised;
    static uint32_t beacon_period_ms;
};

#endif  // SRC_TELECOMMS_RUNNABLE_BEACON_H_
