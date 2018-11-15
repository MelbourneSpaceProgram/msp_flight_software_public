#ifndef SRC_ADCS_RUNNABLE_ORIENTATION_CONTROL_H_
#define SRC_ADCS_RUNNABLE_ORIENTATION_CONTROL_H_

#include <src/tasks/runnable.h>
#include <ti/sysbios/BIOS.h>
#include <ti/sysbios/knl/Semaphore.h>

class RunnableOrientationControl : public Runnable {
   public:
    fnptr GetRunnablePointer();
    RunnableOrientationControl();
    static void SetupControlLoopTimer();
    static void OrientationControlTimerISR(
        UArg orientation_control_timer_semaphore);

   private:
    static void ControlOrientation();

    static Semaphore_Handle control_loop_timer_semaphore;
};

#endif  // SRC_ADCS_RUNNABLE_ORIENTATION_CONTROL_H_
