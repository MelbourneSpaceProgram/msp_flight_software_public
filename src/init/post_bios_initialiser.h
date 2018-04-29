#ifndef SRC_INIT_POST_BIOS_INITIALISER_H_
#define SRC_INIT_POST_BIOS_INITIALISER_H_

#include <src/i2c/i2c.h>
#include <src/tasks/runnable.h>
#include <src/tasks/task_holder.h>
#include <xdc/runtime/System.h>

class PostBiosInitialiser : public Runnable {
   public:
    PostBiosInitialiser();
    fnptr GetRunnablePointer();

   private:
    // TODO(dingbenjamin): Suppress unused function warning
    static void PostBiosInit();
    static void InitSingletons(I2c* bus_a, I2c* bus_b, I2c* bus_c, I2c* bus_d);
    static void InitRadioListener();
    static void InitPayloadProcessor();
    static void InitStateManagement();
    static void InitDataDashboard();
    static void InitOrientationControl();
    static TaskHolder* InitPreDeploymentMagnetometerPoller();
    static void InitBeacon();
    static void RunUnitTests();
    static void InitHardware();
    static void DeploymentWait(uint16_t delay);
    static void DeployAntenna();
    static void InitMemoryLogger();
    static void InitTimeSource();

    static const uint16_t kDelayCheckInterval = 60000;
    static const uint16_t kSecsInMin = 60;
    static const uint16_t kBeaconDelay = 30;
    static const uint16_t kAntennaDelay = 90;

};

#endif  // SRC_INIT_POST_BIOS_INITIALISER_H_
