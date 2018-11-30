#include <external/wmm/worldMagneticModel.h>
#include <src/adcs/controllers/b_dot_controller.h>
#include <src/adcs/controllers/nadir_controller.h>
#include <src/adcs/magnetorquer_control.h>
#include <src/adcs/runnable_orientation_control.h>
#include <src/adcs/state_estimators/nadir_error_generator.h>
#include <src/board/debug_interface/debug_stream.h>
#include <src/config/orientation_control_tuning_parameters.h>
#include <src/config/satellite.h>
#include <src/messages/BDotEstimate.pb.h>
#include <src/messages/GyroscopeReading.pb.h>
#include <src/messages/MagnetometerReading.pb.h>
#include <src/messages/TorqueOutputReading.pb.h>
#include <src/sensors/i2c_sensors/measurables/imu_magnetometer_measurable.h>
#include <src/sensors/measurable_id.h>
#include <src/sensors/measurable_manager.h>
#include <src/util/message_codes.h>
#include <src/util/msp_exception.h>
#include <src/util/task_utils.h>
#include <ti/sysbios/hal/Timer.h>
#include <xdc/runtime/Log.h>

Semaphore_Handle RunnableOrientationControl::control_loop_timer_semaphore;

BDotEstimator RunnableOrientationControl::b_dot_estimator(
    kOrientationControlLoopPeriodMicros * 1e-3,
    kBDotEstimatorTimeConstantMillis);
FirstOrderIirLowpass RunnableOrientationControl::angular_velocity_smoother(
    kOrientationControlLoopPeriodMicros * 1e-3,
    kBDotEstimatorTimeConstantMillis);
EarthSensor RunnableOrientationControl::earth_sensor;
LocationEstimator RunnableOrientationControl::location_estimator;
KalmanFilter RunnableOrientationControl::kalman_filter(
    kOrientationControlLoopPeriodMicros, kKFProcessNoise, kKFSensorNoise,
    kKFInitialCovariance, kKFInitialEstimate);

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
        kOrientationControlLoopPeriodMicros;
    orientation_control_timer_params.arg =
        (UArg)RunnableOrientationControl::control_loop_timer_semaphore;
    orientation_control_timer = Timer_create(
        Board_TIMER2, RunnableOrientationControl::OrientationControlTimerISR,
        &orientation_control_timer_params, NULL);
    if (orientation_control_timer == NULL) {
        throw MspException("Timer create failed",
                           kRunnableOrientationControlTimerFail, __FILE__,
                           __LINE__);
    }
}

void RunnableOrientationControl::OrientationControlTimerISR(
    UArg orientation_control_timer_semaphore) {
    Semaphore_post((Semaphore_Handle)orientation_control_timer_semaphore);
}

void RunnableOrientationControl::ControlOrientation() {
    MeasurableManager* measurable_manager = MeasurableManager::GetInstance();

    while (1) {
        try {
            // Check if we have a valid estimate of our position in space.
            // Our position is not sensitive to timing at the level of
            // milliseconds, so we can propagate our orbit with SGP4 during idle
            // time, before pending on the timer semaphore.
            bool location_known = location_estimator.UpdateLocation();

            Semaphore_pend(control_loop_timer_semaphore, BIOS_WAIT_FOREVER);

            // TODO(dingbenjamin): Check if we should turn orientation control
            // off

            MagnetorquerControl::Degauss();

            GyroscopeReading gyro_reading_pb =
                measurable_manager->ReadNanopbMeasurable<GyroscopeReading>(
                    kFsImuGyro1, 0);
            if (kHilAvailable) {
                // Echo gyro reading to DebugClient
                PostNanopbToSimMacro(GyroscopeReading, kGyroscopeReadingCode,
                                     gyro_reading_pb);
            }

            NewStackMatrixMacro(gyro_reading, 3, 1);
            gyro_reading.FromNanopbXYZ<GyroscopeReading>(gyro_reading_pb);
            double gyro_norm = Matrix::VectorNorm(gyro_reading);
            double angular_velocity_smoothed =
                angular_velocity_smoother.ProcessSample(gyro_norm);
            bool spinning_too_fast_for_orientation_control =
                angular_velocity_smoothed > kCriticallyFastSpin;
            bool spinning_slow_enough_for_nadir_pointing =
                angular_velocity_smoothed < kSpinSlowEnoughForEarthPointing;

            // Choose appropriate control algorithm for computing magnetorquer
            // actuation
            NewStackMatrixMacro(signed_pwm_output, 3, 1);
            bool success;
            if (location_known && spinning_slow_enough_for_nadir_pointing) {
                success = PointToNadir(gyro_reading, signed_pwm_output);
            } else {
                success = Detumble(signed_pwm_output);
            }
            if (!success) {
                continue;
            }

            // Scale actuation strength for power budgeting
            for (uint8_t i = 0; i < 3; i++) {
                signed_pwm_output.Set(i, 0,
                                      signed_pwm_output.Get(i, 0) *
                                          kOrientationControlPowerLevel);
            }

            MagnetorquerControl::SetMagnetorquersPowerFraction(
                signed_pwm_output.Get(0, 0), signed_pwm_output.Get(1, 0),
                signed_pwm_output.Get(2, 0));

        } catch (MspException& e) {
            MspException::LogTopLevelException(
                e, kRunnableOrientationControlCatch);
        }
    }
}

bool RunnableOrientationControl::Detumble(Matrix& signed_pwm_output) {
    MeasurableManager* measurable_manager = MeasurableManager::GetInstance();
    // TODO (rskew) fuse readings from both magnetometers giving redundancy
    // TODO (rskew) handle exception from magnetometer overflow
    MagnetometerReading magnetometer_reading =
        measurable_manager->ReadNanopbMeasurable<MagnetometerReading>(
            kFsImuMagno2, 0);

    NewStackMatrixMacro(b_dot_estimate, 3, 1);
    // Failed readings return a value of (-9999.0,-9999.0,-9999.0) which
    // wind up the BDotEstimator.
    if (magnetometer_reading.x ==
        measurable_manager->GetMeasurable<MagnetometerReading>(kFsImuMagno2)
            ->GetFailureReading()
            .x) {
        return false;
    }

    b_dot_estimator.Estimate(magnetometer_reading, b_dot_estimate);

    if (kHilAvailable) {
        BDotEstimate b_dot_estimate_pb;
        b_dot_estimate.ToNanopbXYZ<BDotEstimate>(b_dot_estimate_pb);
        PostNanopbToSimMacro(BDotEstimate, kBDotEstimateCode,
                             b_dot_estimate_pb);
    }

    BDotController::ComputeControl(b_dot_estimate, signed_pwm_output);
    return true;
}

bool RunnableOrientationControl::PointToNadir(Matrix& gyro_reading,
                                              Matrix& signed_pwm_output) {
    MeasurableManager* measurable_manager = MeasurableManager::GetInstance();

    LocationReading location_reading;
    location_estimator.GetLocationReading(location_reading);
    // Reference vectors are in the North East Down (NED) frame.
    r_vector mag_field_reference;
    mag_field_reference =
        MagModel(kMissionYear, location_reading.altitude_above_ellipsoid_km,
                 location_reading.lattitude_geodetic_degrees,
                 location_reading.longitude_degrees);
    NewStackMatrixMacro(geomag_reference, 3, 1);
    geomag_reference.Set(0, 0, mag_field_reference.x);
    geomag_reference.Set(1, 0, mag_field_reference.y);
    geomag_reference.Set(2, 0, mag_field_reference.z);
    kalman_filter.UpdateRef1(geomag_reference);
    // earth vector is always (0,0,1) in NED
    double nadir_reference_data[3][1] = {{0}, {0}, {1}};
    Matrix nadir_reference(nadir_reference_data);
    kalman_filter.UpdateRef2(nadir_reference);

    kalman_filter.Predict(gyro_reading);

    MagnetometerReading magnetometer_reading =
        measurable_manager->ReadNanopbMeasurable<MagnetometerReading>(
            kFsImuMagno2, 0);
    if (magnetometer_reading.x ==
        measurable_manager->GetMeasurable<MagnetometerReading>(kFsImuMagno2)
            ->GetFailureReading()
            .x) {
        return false;
    }

    NewStackMatrixMacro(nadir_reading, 3, 1);
    earth_sensor.CalculateNadirVector(nadir_reading);

    NewStackMatrixMacro(geomag_reading, 3, 1);
    geomag_reading.FromNanopbXYZ(magnetometer_reading);

    kalman_filter.Update(geomag_reading, nadir_reading);

    NewStackMatrixMacro(error_quaternion, 4, 1);
    ErrorQuaternionCalculatorMarkely(nadir_reference, kalman_filter.q_estimate,
                                     error_quaternion);

    NewStackMatrixMacro(desired_torque, 3, 1);
    NadirController::ComputeControl(error_quaternion, gyro_reading,
                                    desired_torque);

    // Convert the desired torque to the desired magnetic field, then to PWM
    NewStackMatrixMacro(geomag_unit, 3, 1);
    // TODO (rskew) check why this is negated
    geomag_unit.MultiplyScalar(
        geomag_reading, -1.0 / geomag_reading.VectorNorm(geomag_reading));
    signed_pwm_output.CrossProduct(geomag_reading, desired_torque);
    return true;
}
