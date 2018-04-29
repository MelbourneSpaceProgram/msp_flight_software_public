#include <external/nanopb/pb_decode.h>
#include <src/adcs/magnetorquer_control.h>
#include <src/adcs/runnable_pre_deployment_magnetometer_poller.h>
#include <src/config/unit_tests.h>
#include <src/debug_interface/debug_stream.h>
#include <src/messages/MagnetometerReading.pb.h>
#include <src/sensors/specific_sensors/magnetometer.h>
#include <src/util/message_codes.h>
#include <src/util/task_utils.h>
#include <ti/sysbios/BIOS.h>

Semaphore_Handle RunnablePreDeploymentMagnetometerPoller::
    kill_task_on_orientation_control_begin_semaphore;

fnptr RunnablePreDeploymentMagnetometerPoller::GetRunnablePointer() {
    return &RunnablePreDeploymentMagnetometerPoller::PollMagnetometer;
}

void RunnablePreDeploymentMagnetometerPoller::
    SetupKillTaskOnOrientationControlBeginSemaphore() {
    Semaphore_Params kill_task_on_orientation_control_begin_semaphore_params;
    Semaphore_Params_init(
        &kill_task_on_orientation_control_begin_semaphore_params);
    kill_task_on_orientation_control_begin_semaphore_params.mode =
        Semaphore_Mode_BINARY;
    RunnablePreDeploymentMagnetometerPoller::
        kill_task_on_orientation_control_begin_semaphore = Semaphore_create(
            0, &kill_task_on_orientation_control_begin_semaphore_params, NULL);
}

void RunnablePreDeploymentMagnetometerPoller::PollMagnetometer() {
    DebugStream* debug_stream = DebugStream::GetInstance();
    Magnetometer magnetometer;

    MagnetorquerControl::Degauss();

    while (1) {
        TaskUtils::SleepMilli(kSleepPeriodMillis);

        // Kill this task when orientation control boots up to avoid
        // multiple tasks controlling the magnetorquers.
        // Semaphore_pend returns false when it times out and true
        // when the semaphore is set.
        bool orientation_control_begin = Semaphore_pend(
            kill_task_on_orientation_control_begin_semaphore, BIOS_NO_WAIT);
        if (orientation_control_begin) {
            Task_exit();
        }

        // Poll magnetometer, which internally writes new readings
        // to a persistant buffer backed by a file on the SD card.
        // TODO(rskew) handle false return value
        bool success = magnetometer.TakeReading();
        if (!success) {
            continue;
        }
    }
}
