#include <Board.h>
#include <external/nanopb/pb_decode.h>
#include <math.h>
#include <src/adcs/controllers/b_dot_controller.h>
#include <src/adcs/magnetorquer_control.h>
#include <src/adcs/runnable_orientation_control.h>
#include <src/adcs/runnable_pre_deployment_magnetometer_poller.h>
#include <src/adcs/state_estimators/b_dot_estimator.h>
#include <src/adcs/state_estimators/location_estimator.h>
#include <src/config/unit_tests.h>
#include <src/data_dashboard/runnable_data_dashboard.h>
#include <src/debug_interface/debug_stream.h>
#include <src/init/init.h>
#include <src/messages/GyrometerReading.pb.h>
#include <src/messages/LocationReading.pb.h>
#include <src/messages/MagnetometerReading.pb.h>
#include <src/messages/Time.pb.h>
#include <src/messages/Tle.pb.h>
#include <src/messages/TorqueOutputReading.pb.h>
#include <src/sensors/specific_sensors/gyrometer.h>
#include <src/sensors/specific_sensors/magnetometer.h>
#include <src/util/message_codes.h>
#include <src/util/satellite_time_source.h>
#include <src/util/task_utils.h>
#include <ti/sysbios/BIOS.h>
#include <ti/sysbios/hal/Timer.h>

Semaphore_Handle RunnableOrientationControl::control_loop_timer_semaphore;

I2c* RunnableOrientationControl::i2c_bus_a;

/* TODO(rskew) update inertia matrix. */
double RunnableOrientationControl::acrux1_inertia_matrix_data[3][3] =
    /* kg.m^2 */
    {{1191.648 * 1.3e-6, 0, 0},
     {0, 1169.506 * 1.3e-6, 0},
     {0, 0, 1203.969 * 1.3e-6}};

const Matrix RunnableOrientationControl::acrux1_inertia_matrix(
    RunnableOrientationControl::acrux1_inertia_matrix_data);

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
    Semaphore_post(control_loop_timer_semaphore);
}

void RunnableOrientationControl::ControlOrientation() {
    DebugStream* debug_stream = DebugStream::GetInstance();
    Magnetometer magnetometer(i2c_bus_a);
    Gyrometer gyrometer;
    BDotEstimator b_dot_estimator(50, 4000);
    LocationEstimator location_estimator;

    bool successful_calibration = magnetometer.Calibrate();
    if (!successful_calibration) {
        // Try once more before just starting anyway
        //TaskUtils::SleepMilli(kMagCalRetrySleepPeriodMillis);
        //magnetometer.Calibrate();
      etl::exception e("Magnetometer calibration failed", __FILE__, __LINE__);
      throw e;
    }
    Semaphore_post(RunnablePreDeploymentMagnetometerPoller::
                       kill_task_on_orientation_control_begin_semaphore);

    while (1) {
        Semaphore_pend(control_loop_timer_semaphore, BIOS_WAIT_FOREVER);

        // TODO(rskew) switch algorithms based on AdcsStateMachine state
        TaskUtils::SleepMilli(2000);

        MagnetorquerControl::Degauss();

        ////////////////
        // State Observer section
        ////////////////

        // Read Magnetometer
        // TODO(rskew) handle false return value
        bool success = magnetometer.TakeReading();
        if (!success) {
            continue;
        }
        MagnetometerReading magnetometer_reading = magnetometer.GetReading();

        // Run estimator
        double geomag_data[3][1] = {{magnetometer_reading.x},
                                    {magnetometer_reading.y},
                                    {magnetometer_reading.z}};
        Matrix geomag(geomag_data);
        double b_dot_estimate_data[3][1];
        Matrix b_dot_estimate(b_dot_estimate_data);
        b_dot_estimator.Estimate(geomag, b_dot_estimate);

        // Read Gyrometer
        // TODO(rskew) handle false return value
        //success = gyrometer.TakeReading();
        //if (!success) {
        //    continue;
        //}
        //GyrometerReading gyrometer_reading = gyrometer.GetReading();
        //
        //// Calculate angular momentum for debugging (and momentum dumping
        //// detumbling?)
        //double angular_velocity_data[3][1];
        //Matrix angular_velocity(angular_velocity_data);
        //angular_velocity.Set(0, 0, gyrometer_reading.x);
        //angular_velocity.Set(1, 0, gyrometer_reading.y);
        //angular_velocity.Set(2, 0, gyrometer_reading.z);
        //double angular_momentum_data[3][1];
        //Matrix angular_momentum(angular_momentum_data);
        //angular_momentum.Multiply(
        //    RunnableOrientationControl::acrux1_inertia_matrix,
        //    angular_velocity);

        // TODO(rskew) tell DetumbledStateMachine about Bdot

        //if (hil_enabled) {
        //    // This all gets updated in the tle-uplink branch

        //    DebugStream* debug_stream = DebugStream::GetInstance();
        //    uint8_t buffer[Tle_size];
        //    bool success = debug_stream->RequestMessageFromSimulator(
        //        kTleRequestCode, buffer, Tle_size);
        //    if (success) {
        //        pb_istream_t stream = pb_istream_from_buffer(buffer, Tle_size);
        //        Tle tle = Tle_init_zero;
        //        bool status = pb_decode(&stream, Tle_fields, &tle);
        //        if (!status) {
        //            etl::exception e("pb_decode failed", __FILE__, __LINE__);
        //            throw e;
        //        }

        //        location_estimator.StoreTle(tle);

        //        // Calculate position
        //        // TODO (rskew) calculate absolute time in millis since tle
        //        // epoch
        //        Time satellite_time = SatelliteTimeSource::GetTime();
        //        if (!satellite_time.is_valid) {
        //            // TODO (rskew) update TleStateMachine
        //            continue;
        //        }
        //        location_estimator.UpdateLocation(
        //            satellite_time.timestamp_millis_unix_epoch);

        //        // Write calculated position to data dashboard
        //        LocationReading location_reading = LocationReading_init_zero;
        //        location_reading.lattitude_geodetic_degrees =
        //            location_estimator.GetLattitudeGeodeticDegrees();
        //        location_reading.longitude_degrees =
        //            location_estimator.GetLongitudeDegrees();
        //        location_reading.altitude_above_ellipsoid_km =
        //            location_estimator.GetAltitudeAboveEllipsoidKm();
        //        // TODO (rskew) generate timestamp
        //        location_reading.timestamp_millis_unix_epoch = 0;
        //        RunnableDataDashboard::TransmitMessage(
        //            kLocationReadingCode, LocationReading_size,
        //            LocationReading_fields, &location_reading);
        //    }
        //}

        ////////////////
        // Controller section
        ////////////////

        // Detumbling controller
        // TODO (rskew) controller output PWM
        double torque_output_data[3][1];
        Matrix torque_output(torque_output_data);
        BDotController::Control(geomag, b_dot_estimate, torque_output);
        float torque_boost = 1e-2f;
        MagnetorquerControl::SetMagnetorquersPowerFraction(
            torque_output.Get(0, 0) * torque_boost,
            torque_output.Get(1, 0) * torque_boost,
            torque_output.Get(2, 0) * torque_boost);
    }
}
