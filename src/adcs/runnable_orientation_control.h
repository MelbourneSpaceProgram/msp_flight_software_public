#ifndef SRC_ADCS_RUNNABLE_ORIENTATION_CONTROL_H_
#define SRC_ADCS_RUNNABLE_ORIENTATION_CONTROL_H_

#include <external/nanopb/pb.h>
#include <src/tasks/runnable.h>
#include <src/util/data_types.h>
#include <src/util/matrix.h>
#include <ti/sysbios/BIOS.h>
#include <ti/sysbios/knl/Semaphore.h>
#include <src/i2c/i2c.h>

class RunnableOrientationControl : public Runnable {
   public:
    fnptr GetRunnablePointer();
    RunnableOrientationControl();
    static void SetupControlLoopTimer();
    static void OrientationControlTimerISR(
        UArg orientation_control_timer_semaphore);

    static const uint32_t kControlLoopPeriodMicros = 1000000;
    static double acrux1_inertia_matrix_data[3][3];
    static const Matrix acrux1_inertia_matrix;

    static I2c* i2c_bus_a;

   private:
    static void ControlOrientation();

    static Semaphore_Handle control_loop_timer_semaphore;

    static const uint32_t kMagCalRetrySleepPeriodMillis = 1 * 60 * 60 * 1000;
};

#endif  // SRC_ADCS_RUNNABLE_ORIENTATION_CONTROL_H_
