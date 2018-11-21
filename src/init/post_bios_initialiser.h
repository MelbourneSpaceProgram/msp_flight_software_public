#ifndef SRC_INIT_POST_BIOS_INITIALISER_H_
#define SRC_INIT_POST_BIOS_INITIALISER_H_

#include <src/config/stacks.h>
#include <src/tasks/runnable.h>
#include <src/util/data_types.h>
#include <xdc/runtime/System.h>

class TaskHolder;
class I2c;
class Uart;

class PostBiosInitialiser : public Runnable {
    friend void EnterLimpMode();

   public:
    PostBiosInitialiser();
    fnptr GetRunnablePointer();

   private:
    static void PostBiosInit();

    static void InitConsoleUart(
        uint16_t logger_stack_size = kConsoleLoggerStackSize,
        uint16_t listener_stack_size = kConsoleListenerStackSize);
    static void InitRadioListener(
        uint16_t stack_size = kRadioListenerStackSize);
    static void InitPayloadProcessor(
        uint16_t stack_size = kPayloadProcessorStackSize);
    static void InitOrientationControl(
        uint16_t stack_size = kOrientationControlStackSize);
    static void InitSystemHealthCheck(
        uint16_t stack_size = kSystemHealthCheckStackSize);
    static void InitBeacon(uint16_t stack_size = kBeaconStackSize);
    static void InitAntennaBurner(
        uint16_t stack_size = kAntennaBurnerStackSize);
    static void InitMemoryLogger(uint16_t stack_size = kMemoryLoggerStackSize);
    static void InitTimeSource(uint16_t stack_size = kTimeSourceStackSize);
    static void InitContinuousTransmitShutoff(
        uint16_t stack_size = kTransmitShutoffStackSize);
    static void InitPowerManager(uint16_t stack_size = kPowerManagerStackSize);
    static void RunUnitTests(uint16_t stack_size = kUnitTestsStackSize);

    static void InitSingletons(I2c* bus_a, I2c* bus_b, I2c* bus_c, I2c* bus_d);
    static Uart* InitDebugUart();
    static void InitHardware();
    static void EjectionWait();
    static void BeaconWait();

    static constexpr uint16_t kDebugUartReadTimeout = 1000;
    static constexpr uint16_t kDebugUartWriteTimeout = 1000;
};

#endif  // SRC_INIT_POST_BIOS_INITIALISER_H_
