#ifndef SRC_ADCS_RUNNABLE_PRE_DEPLOYMENT_MAGNETOMETER_POLLER_H_
#define SRC_ADCS_RUNNABLE_PRE_DEPLOYMENT_MAGNETOMETER_POLLER_H_

#include <src/tasks/runnable.h>
#include <ti/sysbios/knl/Semaphore.h>

class RunnablePreDeploymentMagnetometerPoller : public Runnable {
   public:
    fnptr GetRunnablePointer();
    RunnableMagnetometerPoller(){};
    static void SetupKillTaskOnOrientationControlBeginSemaphore();

    static Semaphore_Handle RunnablePreDeploymentMagnetometerPoller::
        kill_task_on_orientation_control_begin_semaphore;

   private:
    static void PollMagnetometer();

    // Want to collect ~1000 samples in just under 2 hours.
    //     total_seconds * safety_factor / n_samples = polling_period_seconds
    //     2*60*60 * 0.95 / 1000 = 6.84 seconds
    static const uint16_t kSleepPeriodMillis = 6840;
};

#endif  // SRC_ADCS_RUNNABLE_PRE_DEPLOYMENT_MAGNETOMETER_POLLER_H_
