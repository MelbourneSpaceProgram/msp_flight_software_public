#ifndef SRC_INIT_POST_BIOS_INITIALISER_H_
#define SRC_INIT_POST_BIOS_INITIALISER_H_

#include <src/i2c/i2c.h>
#include <src/tasks/runnable.h>

class PostBiosInitialiser : public Runnable {
   public:
    PostBiosInitialiser();
    fnptr GetRunnablePointer();

   private:
    // TODO(dingbenjamin): Suppress unused function warning
    static void PostBiosInit();
    static void InitSingletons(I2c* bus_a, I2c* bus_b, I2c* bus_c, I2c* bus_d);
    static void InitRadioListener();
    static void InitStateManagement();
    static void InitDataDashboard();
    static void OrientationControlTimerISR(UArg timer_semaphore);
    static void InitOrientationControl();
    static void InitBeacon();
    static void RunUnitTests();
    static void InitHardware();
    static void DeploymentWait();
    static void DeployAntenna();

    static const uint16_t kDelayCheckInterval = 60000;
};

#endif  // SRC_INIT_POST_BIOS_INITIALISER_H_
