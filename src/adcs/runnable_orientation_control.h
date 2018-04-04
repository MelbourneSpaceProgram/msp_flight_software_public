#ifndef SRC_ADCS_RUNNABLE_ORIENTATION_CONTROL_H_
#define SRC_ADCS_RUNNABLE_ORIENTATION_CONTROL_H_

#include <external/nanopb/pb.h>
#include <src/tasks/runnable.h>
#include <src/util/data_types.h>
#include <ti/sysbios/knl/Semaphore.h>

class RunnableOrientationControl : public Runnable {
   public:
    fnptr GetRunnablePointer();
    RunnableOrientationControl();
    void SetTimerSemaphore(Semaphore_Handle semaphore);

   private:
    static void ControlOrientation();
    static Semaphore_Handle timer_semaphore;
};

#endif  // SRC_ADCS_RUNNABLE_ORIENTATION_CONTROL_H_
