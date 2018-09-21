#include <external/nanopb/pb_decode.h>
#include <src/adcs/magnetorquer_control.h>
#include <src/adcs/runnable_pre_deployment_magnetometer_poller.h>
#include <src/board/debug_interface/debug_stream.h>
#include <src/config/unit_tests.h>
#include <src/messages/MagnetometerReading.pb.h>
#include <src/sensors/i2c_measurable_manager.h>
#include <src/sensors/measurable_id.h>
#include <src/util/message_codes.h>
#include <src/util/task_utils.h>
#include <ti/sysbios/BIOS.h>
#include <ti/sysbios/knl/Task.h>
#include <xdc/runtime/Log.h>

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
    I2cMeasurableManager* measurable_manager =
        I2cMeasurableManager::GetInstance();

    MagnetorquerControl::Degauss();

    while (1) {
        TaskUtils::SleepMilli(kSleepPeriodMillis);

        // Kill this task when orientation control boots up to avoid
        // multiple tasks controlling the magnetorquers.
        // Semaphore_pend returns false when it times out and true
        // when the semaphore is set.
        bool orientation_control_has_begun = Semaphore_pend(
            kill_task_on_orientation_control_begin_semaphore, BIOS_NO_WAIT);
        if (orientation_control_has_begun) {
            Log_info0("Exiting pre-deployment magnetometer poller.");
            Task_exit();
        }

        // Poll magnetometers, which internally write new readings
        // to persistant buffers backed by files on the SD card
        // for the sake of calibration
        measurable_manager->ReadI2cMeasurable<MagnetometerReading>(kFsImuMagno1,
                                                                   0);
        measurable_manager->ReadI2cMeasurable<MagnetometerReading>(kFsImuMagno2,
                                                                   0);
    }
}
