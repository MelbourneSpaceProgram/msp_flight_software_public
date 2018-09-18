#ifndef SRC_INIT_POST_BIOS_INITIALISER_H_
#define SRC_INIT_POST_BIOS_INITIALISER_H_

#include <src/tasks/runnable.h>
#include <xdc/runtime/System.h>

class TaskHolder;
class I2c;
class Uart;

class PostBiosInitialiser : public Runnable {
   public:
    PostBiosInitialiser();
    fnptr GetRunnablePointer();

   private:
    static void PostBiosInit();
    static Uart* InitDebugUart();
    static void InitSingletons(I2c* bus_a, I2c* bus_b, I2c* bus_c, I2c* bus_d);
    static void InitRadioListener();
    static void InitPayloadProcessor();
    static void InitStateManagement();
    static void InitDataDashboard();
    static void InitOrientationControl();
    static void InitSystemHealthCheck(Uart* debug_uart);
    static void InitConsoleUartListener(Uart* debug_uart);
    static void InitPreDeploymentMagnetometerPoller();
    static void InitBeacon();
    static void RunUnitTests();
    static void InitHardware();
    static void DeployAntenna();
    static void InitMemoryLogger();
    static void InitTimeSource();

    static const uint16_t kDebugUartReadTimeout = 1000;
    static const uint16_t kDebugUartWriteTimeout = 1000;
    static const uint16_t kSecsInMin = 60;
    static const uint16_t kBeaconDelayMins = 30;
    static const uint16_t kBeaconDelaySeconds = kBeaconDelayMins * kSecsInMin;
    static const uint16_t kAntennaDelayMins = 90;
    static const uint16_t kAntennaDelaySeconds = kAntennaDelayMins * kSecsInMin;
};

#endif  // SRC_INIT_POST_BIOS_INITIALISER_H_
