#include <external/nanopb/pb_decode.h>
#include <math.h>
#include <src/adcs/controllers/b_dot_controller.h>
#include <src/adcs/magnetorquer_control.h>
#include <src/adcs/runnable_orientation_control.h>
#include <src/adcs/state_estimators/b_dot_estimator.h>
#include <src/adcs/state_estimators/location_estimator.h>
#include <src/board/board.h>
#include <src/board/debug_interface/debug_stream.h>
#include <src/config/unit_tests.h>
#include <src/data_dashboard/runnable_data_dashboard.h>
#include <src/init/init.h>
#include <src/messages/BDotEstimate.pb.h>
#include <src/messages/LocationReading.pb.h>
#include <src/messages/MagnetometerReading.pb.h>
#include <src/messages/TorqueOutputReading.pb.h>
#include <src/sensors/i2c_measurable_manager.h>
#include <src/sensors/i2c_sensors/measurables/imu_magnetometer_measurable.h>
#include <src/sensors/measurable_id.h>
#include <src/system/state_definitions.h>
#include <src/system/state_manager.h>
#include <src/system/system_state_machines/power_state_machine.h>
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
    BDotEstimator b_dot_estimator(50, 4000);

    StateManager* state_manager = StateManager::GetStateManager();
    I2cMeasurableManager* measurable_manager =
        I2cMeasurableManager::GetInstance();

    if (!(dynamic_cast<ImuMagnetometerMeasurable*>(
              measurable_manager->GetMeasurable<MagnetometerReading>(
                  kFsImuMagno1)))
             ->Calibrate()) {
        Log_error0("Magnetometer on bus A calibration failed");
    }
    if (!(dynamic_cast<ImuMagnetometerMeasurable*>(
              measurable_manager->GetMeasurable<MagnetometerReading>(
                  kFsImuMagno2)))
             ->Calibrate()) {
        Log_error0("Magnetometer on bus B calibration failed");
    }

    while (1) {
        Semaphore_pend(control_loop_timer_semaphore, BIOS_WAIT_FOREVER);

        StateId adcs_state =
            state_manager->GetCurrentStateOfStateMachine(kAdcsStateMachine);

        if (adcs_state == kAdcsOff) {
            Log_warning0("Orientation Control Disabled.");
            // Execution goes back to the semaphore pend and waits for next
            // enable from the timer
            continue;
        }

        // TODO(rskew) switch algorithms based on AdcsStateMachine state

        MagnetorquerControl::Degauss();

        // Read Magnetometer
        // TODO (rskew) fuse readings from both magnetometers giving redundancy
        // TODO(rskew) handle exception from magnetometer overflow
        MagnetometerReading magnetometer_reading =
            measurable_manager->ReadI2cMeasurable<MagnetometerReading>(
                kFsImuMagno2, 0);

        if (kHilAvailable) {
            // Echo magnetometer reading to data dashboard
            RunnableDataDashboard::TransmitMessage(
                kMagnetometerReadingCode, MagnetometerReading_size,
                MagnetometerReading_fields, &magnetometer_reading);
        }

        // Run estimator
        double geomag_data[3][1] = {{magnetometer_reading.x},
                                    {magnetometer_reading.y},
                                    {magnetometer_reading.z}};
        Matrix geomag(geomag_data);
        double b_dot_estimate_data[3][1];
        Matrix b_dot_estimate(b_dot_estimate_data);
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
            RunnableDataDashboard::TransmitMessage(
                kBDotEstimateCode, BDotEstimate_size, BDotEstimate_fields,
                &b_dot_estimate_pb);
        }

        // TODO(rskew) tell DetumbledStateMachine about Bdot (or omega?)

        // Run controller
        double signed_pwm_output_data[3][1];
        Matrix signed_pwm_output(signed_pwm_output_data);
        BDotController::ComputeControl(b_dot_estimate, signed_pwm_output);

        // Use magnetorquer driver to set magnetorquer power.
        // Driver input power range should be [-1, 1]

        MagnetorquerControl::SetMagnetorquersPowerFraction(
            signed_pwm_output.Get(0, 0), signed_pwm_output.Get(1, 0),
            signed_pwm_output.Get(2, 0));
    }
}
