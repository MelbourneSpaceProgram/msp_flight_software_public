#include <Board.h>
#include <external/nanopb/pb_decode.h>
#include <math.h>
#include <src/adcs/controllers/b_dot_controller.h>
#include <src/adcs/magnetorquer_control.h>
#include <src/adcs/runnable_orientation_control.h>
#include <src/adcs/state_estimators/b_dot_estimator.h>
#include <src/adcs/state_estimators/location_estimator.h>
#include <src/config/unit_tests.h>
#include <src/data_dashboard/runnable_data_dashboard.h>
#include <src/debug_interface/debug_stream.h>
#include <src/init/init.h>
#include <src/messages/LocationReading.pb.h>
#include <src/messages/MagnetometerReading.pb.h>
#include <src/messages/TorqueOutputReading.pb.h>
#include <src/sensors/specific_sensors/magnetometer.h>
#include <src/system/state_definitions.h>
#include <src/system/state_manager.h>
#include <src/system/system_state_machines/power_state_machine.h>
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
    DebugStream* debug_stream = DebugStream::GetInstance();
    Magnetometer magnetometer;
    BDotEstimator b_dot_estimator(50, 4000);
    LocationEstimator location_estimator;

    StateManager* state_manager = StateManager::GetStateManager();

    Time tle_last_updated;
    Time current_time;
    double time_since_tle_updated_millis;

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
        // TODO(rskew) handle false return value
        bool success = magnetometer.TakeReading();
        if (!success) {
            continue;
        }
        MagnetometerReading magnetometer_reading = magnetometer.GetReading();

        if (hil_enabled) {
            // Echo reading to data dashboard
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
        b_dot_estimator.Estimate(geomag, b_dot_estimate);

        // TODO(rskew) tell DetumbledStateMachine about Bdot (or omega?)

        // Run controller
        double torque_output_data[3][1];
        Matrix torque_output(torque_output_data);
        BDotController::Control(geomag, b_dot_estimate, torque_output);

        // Use magnetorquer driver to set magnetorquer power.
        // Driver input power range should be [-1, 1]

        //
        // TODO(crozone):
        //
        // Verify that torque_boost here is correct for driver input range.
        float torque_boost = 100.0f;

        MagnetorquerControl::SetMagnetorquersPowerFraction(
            torque_output.Get(0, 0) * torque_boost,
            torque_output.Get(1, 0) * torque_boost,
            torque_output.Get(2, 0) * torque_boost);

        if (over_the_air_enabled) {
            if (location_estimator.CheckForUpdatedTle()) {
                tle_last_updated = SatelliteTimeSource::GetTime();
            }
        } else if (hil_enabled) {
            location_estimator.RequestTleFromDebugClient();
            tle_last_updated = SatelliteTimeSource::GetTime();
        }

        if (hil_enabled || over_the_air_enabled) {
            // Calculate position
            current_time = SatelliteTimeSource::GetTime();
            time_since_tle_updated_millis =
                current_time.timestamp_millis_unix_epoch -
                tle_last_updated.timestamp_millis_unix_epoch;
            location_estimator.UpdateLocation(time_since_tle_updated_millis);

            // Write calculated position to data dashboard
            LocationReading location_reading = LocationReading_init_zero;
            location_reading.lattitude_geodetic_degrees =
                location_estimator.GetLattitudeGeodeticDegrees();
            location_reading.longitude_degrees =
                location_estimator.GetLongitudeDegrees();
            location_reading.altitude_above_ellipsoid_km =
                location_estimator.GetAltitudeAboveEllipsoidKm();
            // TODO (rskew) generate timestamp
            location_reading.timestamp_millis_unix_epoch =
                current_time.timestamp_millis_unix_epoch;
            RunnableDataDashboard::TransmitMessage(
                kLocationReadingCode, LocationReading_size,
                LocationReading_fields, &location_reading);
        }
    }
}
