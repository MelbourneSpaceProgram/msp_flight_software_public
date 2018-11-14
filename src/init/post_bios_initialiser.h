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
    static void InitOrientationControl();
    static void InitSystemHealthCheck();
    static void InitConsoleUart();
    static void InitBeacon();
    static void RunUnitTests();
    static void InitHardware();
    static void InitAntennaBurner();
    static void InitMemoryLogger();
    static void InitTimeSource();
    static void InitContinuousTransmitShutoff();
    static void EjectionWait();
    static void BeaconWait();

    static constexpr uint16_t kDebugUartReadTimeout = 1000;
    static constexpr uint16_t kDebugUartWriteTimeout = 1000;
    static constexpr uint32_t kBeaconWaitMs = 5 * 60 * 1000;
    static constexpr uint32_t kEjectionWaitMs = 30 * 60 * 1000;
};

#endif  // SRC_INIT_POST_BIOS_INITIALISER_H_
