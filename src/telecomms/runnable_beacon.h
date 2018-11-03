#ifndef SRC_TELECOMMS_RUNNABLE_BEACON_H_
#define SRC_TELECOMMS_RUNNABLE_BEACON_H_

#include <src/tasks/runnable.h>
#include <src/board/i2c/i2c.h>

class LithiumBeaconPeriodCommand;

class RunnableBeacon : public Runnable {
    friend class LithiumBeaconPeriodUplink;

   public:
    RunnableBeacon();
    static uint32_t GetBeaconPeriodMs();
    fnptr GetRunnablePointer();
    static void InitBusD(const I2c *bus_d);

   private:
    static void Beacon();
    static uint32_t beacon_period_ms;
    static I2c *beacon_bus_d;
};

#endif  // SRC_TELECOMMS_RUNNABLE_BEACON_H_
