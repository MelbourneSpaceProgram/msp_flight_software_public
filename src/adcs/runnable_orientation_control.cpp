#include <external/nanopb/pb_decode.h>
#include <external/wmm/worldMagneticModel.h>
#include <math.h>
#include <src/adcs/controllers/b_dot_controller.h>
#include <src/adcs/controllers/nadir_controller.h>
#include <src/adcs/kalman_filter.h>
#include <src/adcs/magnetorquer_control.h>
#include <src/adcs/runnable_orientation_control.h>
#include <src/adcs/state_estimators/b_dot_estimator.h>
#include <src/adcs/state_estimators/location_estimator.h>
#include <src/adcs/state_estimators/nadir_error_generator.h>
#include <src/board/board.h>
#include <src/board/debug_interface/debug_stream.h>
#include <src/config/orientation_control_tuning_parameters.h>
#include <src/config/satellite.h>
#include <src/init/init.h>
#include <src/messages/BDotEstimate.pb.h>
#include <src/messages/GyroscopeReading.pb.h>
#include <src/messages/LocationReading.pb.h>
#include <src/messages/MagnetometerReading.pb.h>
#include <src/messages/TorqueOutputReading.pb.h>
#include <src/sensors/earth_sensor.h>
#include <src/sensors/i2c_sensors/measurables/imu_magnetometer_measurable.h>
#include <src/sensors/i2c_sensors/mpu9250_motion_tracker.h>
#include <src/sensors/measurable_id.h>
#include <src/sensors/measurable_manager.h>
#include <src/util/matrix.h>
#include <src/util/message_codes.h>
#include <src/util/msp_exception.h>
#include <src/util/satellite_time_source.h>
#include <src/util/task_utils.h>
#include <ti/sysbios/BIOS.h>
#include <ti/sysbios/hal/Timer.h>
#include <xdc/runtime/Log.h>
#include <src/adcs/kalman_filter.h>
#include <src/sensors/i2c_sensors/mpu9250_motion_tracker.h>
#include <src/messages/GyroscopeReading.pb.h>
#include <extern/wmm/worldMagneticModel.h>
#include <src/sensors/earth_sensor.h>
#include <src/adcs/state_estimators/nadir_error_generator.h>

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
    /* Location estimator object*/

    /* Kalman Filter*/

    BDotEstimator b_dot_estimator(
        RunnableOrientationControl::kControlLoopPeriodMicros * 1e-3,
        kBDotEstimatorTimeConstantMillis);

    FirstOrderIirLowpass gyro_filter(
      RunnableOrientationControl::kControlLoopPeriodMicros*1e-3,
      kBDotEstimatorTimeConstantMillis);

    MeasurableManager* measurable_manager = MeasurableManager::GetInstance();
    BDotEstimate b_dot_estimate_pb;
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
    double tsince_millis = 250; // TODO: {jmcrobbie} fix dis
    bool check_tle = false;
    NewStackMatrixMacro(b_dot_estimate, 3, 1);
    NewStackMatrixMacro(geomag, 3, 1);
    NewStackMatrixMacro(gyro, 3, 1);
    NewStackMatrixMacro(gyro_smooth,3,1);
    double gyro_norm;

    /* Matrices for the kalman filter:*/
    NewStackMatrixMacro(r1,3,1);
    NewStackMatrixMacro(r2,3,1);

    double q0_data[4][1] = {{0}, {0}, {0}, {1}};
    Matrix q0(q0_data);

    NewStackMatrixMacro(P0, 3, 3);
    P0.Fill(0);
    NewStackMatrixMacro(Q0, 3, 3);
    Q0.Identity();
    Q0.MultiplyScalar(Q0, 0.1);
    double R0_data[6][6];

    Matrix R0(R0_data);
    R0.Identity();
    R0.MultiplyScalar(R0, 0.1);

    NewStackMatrixMacro(y,6,1);

    NewStackMatrixMacro(nadir,3,1);

    /* Initialise location estimation variables*/
    LocationEstimator location_est;
    double alt, longitude, lat;

    /* Initialise wmm output variables*/
    r_vector mag_field;

   /* Earth Sensor Class*/
   EarthSensor earth_sensor;

   /*Controller matrices*/
   NewStackMatrixMacro(error_q,4,1);
   NewStackMatrixMacro(ideal_torque,3,1);
    while (1) {
        /* If TLE hasnt been received yet*/
        if (check_tle || location_est.CheckForUpdatedTle()){
            check_tle = true;
            location_est.UpdateLocation(tsince_millis);

        }
        Semaphore_pend(control_loop_timer_semaphore, BIOS_WAIT_FOREVER);
        /* If TLE hasnt been received yet*/
        if (check_tle || location_est.CheckForUpdatedTle()) {
            check_tle = true;
            location_est.UpdateLocation(tsince_millis);
        }

            // TODO(dingbenjamin): Check if we should turn orientation control
            // off

        // TODO(rskew) switch algorithms based on the orientation

        MagnetorquerControl::Degauss();

        // Read Magnetometer
        // TODO (rskew) fuse readings from both magnetometers giving redundancy
        // TODO(rskew) handle exception from magnetometer overflow

        MagnetometerReading magnetometer_reading =
            measurable_manager->ReadNanopbMeasurable<MagnetometerReading>(
                kFsImuMagno2, 0);
       GyroscopeReading gyro_reading = measurable_manager->ReadNanopbMeasurable<GyroscopeReading>(kFsImuGyro1,0);
        if (kHilAvailable) {
            // Echo magnetometer reading to DebugClient
            PostNanopbToSimMacro(MagnetometerReading, kMagnetometerReadingCode,
                                 magnetometer_reading);
        }

        // Run estimator
        geomag.Set(0, 0, magnetometer_reading.x);
        geomag.Set(1, 0, magnetometer_reading.y);
        geomag.Set(2, 0, magnetometer_reading.z);

        BDotEstimate b_dot_estimate_pb = BDotEstimate_init_zero;

        gyro.Set(0, 0, gyro_reading.x);
        gyro.Set(1, 0, gyro_reading.y);
        gyro.Set(2, 0, gyro_reading.z);
        gyro_norm = Matrix::VectorNorm(gyro);


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

        // Run controller
        NewStackMatrixMacro(signed_pwm_output, 3, 1);

        /*Run advanced pointing if angular rate is below threshold and tle exists*/

        if (gyro_filter.ProcessSample(gyro_norm) <=
        RunnableOrientationControl::gyro_rate_threshold
         && check_tle == true){

           /*Obtain position estimate from sgp4 routine*/
           location_est.UpdateLocation(tsince_millis);
           alt = location_est.GetAltitudeAboveEllipsoidKm();
           longitude = location_est.GetLongitudeDegrees();
           lat = location_est.GetLattitudeGeodeticDegrees();
           /* Obtain estimate of magnetic field*/
           mag_field = MagModel(YEARFRACTION, alt,lat,longitude);
           /*Write to the r1 and r2 vectors*/
           /*r1 and r2 are in the north east vertical frame */
           r1.Set(0,0,mag_field.x);
           r1.Set(1,0,mag_field.y);
           r1.Set(2,0,mag_field.z);
           r2.Set(0,0,0.0);
           r2.Set(0,0,0.0);
           r2.Set(0,0,-1.0); // earth vector is always (0,0,-1) in this frame

           /*This wont work - needs a fix*/
           KalmanFilter kf(kControlLoopPeriodMicros,r1,r2,Q0,R0,P0,q0);
           // todo figure out how to update r1 dynamically!


           kf.predict(gyro_reading);

           /* Earth Sensor */
           earth_sensor.CalculateNadirVector();
           nadir = earth_sensor.GetNadirVector();
           y.Set(0,0,mag_field.Get(0,0));
           y.Set(1,0,mag_field.Get(1,0));
           y.Set(2,0,mag_field.Get(2,0));
           y.Set(3,0,nadir.Get(0,0));
           y.Set(4,0,nadir.Get(1,0));
           y.Set(5,0,nadir.Get(2,0));

           kf.update(y);

           /*Implement controllers*/
           /*This will need fixing as well!*/
           ErrorQuaternionCalculatorMarkely(&r2,kf.q0,error_q);


        }
        else{ // run bdot
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

}
