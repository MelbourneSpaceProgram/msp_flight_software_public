#ifndef SRC_ADCS_RUNNABLE_PRE_DEPLOYMENT_MAGNETOMETER_POLLER_H_
#define SRC_ADCS_RUNNABLE_PRE_DEPLOYMENT_MAGNETOMETER_POLLER_H_

#include <src/tasks/runnable.h>
#include <ti/sysbios/knl/Semaphore.h>

class RunnablePreDeploymentMagnetometerPoller : public Runnable {
   public:
    fnptr GetRunnablePointer();
    RunnablePreDeploymentMagnetometerPoller(){};

    static void SetupKillTaskOnOrientationControlBeginSemaphore();
    static Semaphore_Handle kill_task_on_orientation_control_begin_semaphore;

   private:
    static void PollMagnetometer();

    static constexpr uint16_t kSleepPeriodMillis = 1000;
};

#endif  // SRC_ADCS_RUNNABLE_PRE_DEPLOYMENT_MAGNETOMETER_POLLER_H_
