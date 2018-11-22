#ifndef SRC_INIT_LIMP_MODE_INITIALISER_H_
#define SRC_INIT_LIMP_MODE_INITIALISER_H_

#include <src/config/stacks.h>
#include <src/tasks/runnable.h>
#include <src/util/data_types.h>
#include <xdc/runtime/System.h>

class LimpModeInitialiser : public Runnable {
   public:
    LimpModeInitialiser();
    fnptr GetRunnablePointer();

   private:
    static void LimpModeInit();
};

#endif  // SRC_INIT_LIMP_MODE_INITIALISER_H_
