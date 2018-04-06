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
    static void InitSingletons(I2c* antenna_bus);
    static void InitRadioListener();
    static void InitStateManagement();
    static void InitDataDashboard();
    static void InitOrientationControl();
    static void RunUnitTests();
    static void InitHardware();
};

#endif  // SRC_INIT_POST_BIOS_INITIALISER_H_
