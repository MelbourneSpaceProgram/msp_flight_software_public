#ifndef SRC_INIT_POST_BIOS_INITIALISER_H_
#define SRC_INIT_POST_BIOS_INITIALISER_H_

#include <src/tasks/runnable.h>

class PostBiosInitialiser : public Runnable {
   public:
    PostBiosInitialiser();
    fnptr GetRunnablePointer();

   private:
    // TODO(dingbenjamin): Suppress unused function warning
    static void PostBiosInit();
};

#endif  // SRC_INIT_POST_BIOS_INITIALISER_H_
