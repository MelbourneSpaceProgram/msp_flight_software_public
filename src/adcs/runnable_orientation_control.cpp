#include <external/nanopb/pb_decode.h>
#include <math.h>
#include <src/adcs/controllers/b_dot_controller.h>
#include <src/adcs/magnetorquer_control.h>
#include <src/adcs/runnable_orientation_control.h>
#include <src/adcs/state_estimators/b_dot_estimator.h>
#include <src/adcs/state_estimators/location_estimator.h>
#include <src/board/board.h>
#include <src/board/debug_interface/debug_stream.h>
#include <src/config/orientation_control_tuning_parameters.h>
#include <src/config/satellite.h>
#include <src/config/unit_tests.h>
#include <src/init/init.h>
#include <src/messages/BDotEstimate.pb.h>
#include <src/messages/LocationReading.pb.h>
#include <src/messages/MagnetometerReading.pb.h>
#include <src/messages/TorqueOutputReading.pb.h>
#include <src/sensors/i2c_sensors/measurables/imu_magnetometer_measurable.h>
#include <src/sensors/measurable_id.h>
#include <src/sensors/measurable_manager.h>
#include <src/util/matrix.h>
#include <src/util/message_codes.h>
#include <src/util/satellite_time_source.h>
#include <src/util/task_utils.h>
#include <ti/sysbios/BIOS.h>
#include <ti/sysbios/hal/Timer.h>
#include <xdc/runtime/Log.h>

Semaphore_Handle RunnableOrientationControl::control_loop_timer_semaphore;

RunnableOrientationControl::RunnableOrientationControl() {}

fnptr RunnableOrientationControl::GetRunnablePointer() {
    return &RunnableOrientationControl::ControlOrientation;
}

void RunnableOrientationControl::SetupControlLoopTimer() {
    Timer_Handle orientation_control_timer;
    // Potential issues with this object going out of scope:
    //   If the timer has parameters changed at runtime and
    //   this object is referred to internally, crashes might happen.
    Timer_Params orientation_control_timer_params;
    Semaphore_Params orientation_control_timer_semaphore_params;
    Semaphore_Params_init(&orientation_control_timer_semaphore_params);
    orientation_control_timer_semaphore_params.mode = Semaphore_Mode_BINARY;
    RunnableOrientationControl::control_loop_timer_semaphore =
        Semaphore_create(0, &orientation_control_timer_semaphore_params, NULL);
    Timer_Params_init(&orientation_control_timer_params);
    orientation_control_timer_params.period =
        RunnableOrientationControl::kControlLoopPeriodMicros;
    orientation_control_timer_params.arg =
        (UArg)RunnableOrientationControl::control_loop_timer_semaphore;
    orientation_control_timer = Timer_create(
        Board_TIMER2, RunnableOrientationControl::OrientationControlTimerISR,
        &orientation_control_timer_params, NULL);
    if (orientation_control_timer == NULL) {
        etl::exception e("Timer create failed", __FILE__, __LINE__);
        throw e;
    }
}

void RunnableOrientationControl::OrientationControlTimerISR(
    UArg orientation_control_timer_semaphore) {
    Semaphore_post((Semaphore_Handle)orientation_control_timer_semaphore);
}

void RunnableOrientationControl::ControlOrientation() {
    BDotEstimator b_dot_estimator(
        RunnableOrientationControl::kControlLoopPeriodMicros * 1e-3,
        kBDotEstimatorTimeConstantMillis);

    MeasurableManager* measurable_manager = MeasurableManager::GetInstance();

    while (1) {
        Semaphore_pend(control_loop_timer_semaphore, BIOS_WAIT_FOREVER);

        // TODO(dingbenjamin): Check if we should turn orientation control off

        // TODO(rskew) switch algorithms based on AdcsStateMachine state

        MagnetorquerControl::Degauss();

        // Read Magnetometer
        // TODO (rskew) fuse readings from both magnetometers giving redundancy
        // TODO(rskew) handle exception from magnetometer overflow
        MagnetometerReading magnetometer_reading =
            measurable_manager->ReadNanopbMeasurable<MagnetometerReading>(
                kFsImuMagno2, 0);

        if (kHilAvailable) {
            // Echo magnetometer reading to DebugClient
            PostNanopbToSimMacro(MagnetometerReading, kMagnetometerReadingCode,
                                 magnetometer_reading);
        }

        // Run estimator

        NewStackMatrixMacro(geomag, 3, 1);
        geomag.Set(0, 0, magnetometer_reading.x);
        geomag.Set(1, 0, magnetometer_reading.y);
        geomag.Set(2, 0, magnetometer_reading.z);
        NewStackMatrixMacro(b_dot_estimate, 3, 1);
        BDotEstimate b_dot_estimate_pb = BDotEstimate_init_zero;

        // Failed readings return a value of (-9999.0,-9999.0,-9999.0) which
        // winds up the BDotEstimator.
        if (geomag.Get(0, 0) !=
            measurable_manager->GetMeasurable<MagnetometerReading>(kFsImuMagno2)
                ->GetFailureReading()
                .x) {
            b_dot_estimator.Estimate(geomag, b_dot_estimate);
            b_dot_estimate_pb.x = b_dot_estimate.Get(0, 0);
            b_dot_estimate_pb.y = b_dot_estimate.Get(1, 0);
            b_dot_estimate_pb.z = b_dot_estimate.Get(2, 0);
        }

        if (kHilAvailable) {
            PostNanopbToSimMacro(BDotEstimate, kBDotEstimateCode,
                                 b_dot_estimate_pb);
        }

        // TODO(rskew) tell DetumbledStateMachine about Bdot (or omega?)

        // Run controller
        NewStackMatrixMacro(signed_pwm_output, 3, 1);
        BDotController::ComputeControl(b_dot_estimate, signed_pwm_output);

        // Scale actuation strength for power budgeting
        for (uint8_t i = 0; i < 3; i++) {
            signed_pwm_output.Set(
                i, 0,
                signed_pwm_output.Get(i, 0) * kOrientationControlPowerLevel);
        }

        // Use magnetorquer driver to set magnetorquer power.
        // Driver input power range should be [-1, 1]

        MagnetorquerControl::SetMagnetorquersPowerFraction(
            signed_pwm_output.Get(0, 0), signed_pwm_output.Get(1, 0),
            signed_pwm_output.Get(2, 0));
    }
}
