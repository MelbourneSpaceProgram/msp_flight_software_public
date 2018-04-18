#include <external/nanopb/pb_decode.h>
#include <math.h>
#include <src/adcs/controllers/b_dot_controller.h>
#include <src/adcs/runnable_orientation_control.h>
#include <src/adcs/state_estimators/b_dot_estimator.h>
#include <src/adcs/state_estimators/location_estimator.h>
#include <src/config/unit_tests.h>
#include <src/data_dashboard/runnable_data_dashboard.h>
#include <src/debug_interface/debug_stream.h>
#include <src/init/init.h>
#include <src/messages/LocationReading.pb.h>
#include <src/messages/MagnetometerReading.pb.h>
#include <src/messages/Tle.pb.h>
#include <src/messages/TorqueOutputReading.pb.h>
#include <src/sensors/specific_sensors/magnetometer.h>
#include <src/util/message_codes.h>
#include <ti/sysbios/BIOS.h>

Semaphore_Handle RunnableOrientationControl::timer_semaphore;

RunnableOrientationControl::RunnableOrientationControl() {}

fnptr RunnableOrientationControl::GetRunnablePointer() {
    return &RunnableOrientationControl::ControlOrientation;
}

void RunnableOrientationControl::SetTimerSemaphore(Semaphore_Handle semaphore) {
    timer_semaphore = semaphore;
}

void RunnableOrientationControl::ControlOrientation() {
    DebugStream* debug_stream = DebugStream::GetInstance();
    Magnetometer magnetometer("asdf");
    BDotEstimator b_dot_estimator(50, 4000);
    LocationEstimator location_estimator;

    // TODO (rskew) replace this with actual rtc time
    double tsince_millis = 0;

    while (1) {
        Semaphore_pend(timer_semaphore, BIOS_WAIT_FOREVER);

        // TODO(rskew) switch algorithms based on AdcsStateMachine state

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
         BDotController::Control(geomag, b_dot_estimate,
                                torque_output);

        // TODO (rskew) create magnetorquer abstraction that wraps the following
        // ---------- To be wrapped by magnetorquer abstraction ----------
        // TODO (rskew) set magnetorquer PWM width based on control output

        if (hil_enabled) {
            // Send torque output to simulation
            TorqueOutputReading torque_output_reading =
                TorqueOutputReading_init_zero;
            // torque_output_reading.x = torque_output.Get(0, 0);
            // torque_output_reading.y = torque_output.Get(1, 0);
            // torque_output_reading.z = torque_output.Get(2, 0);
            // Torque boost woohoo
            double torque_boost = 100;
            torque_output_reading.x = torque_boost * torque_output.Get(0, 0);
            torque_output_reading.y = torque_boost * torque_output.Get(1, 0);
            torque_output_reading.z = torque_boost * torque_output.Get(2, 0);
            RunnableDataDashboard::TransmitMessage(
                kTorqueOutputReadingCode, TorqueOutputReading_size,
                TorqueOutputReading_fields, &torque_output_reading);
        }
        // ---------- end section to be wrapped ----------

        if (hil_enabled) {
            // TODO (rskew) move this code to a command handler, allowing the
            // TLE update to be driven by the ground station.

            DebugStream* debug_stream = DebugStream::GetInstance();
            uint8_t buffer[Tle_size];
            bool success = debug_stream->RequestMessageFromSimulator(
                kTleRequestCode, buffer, Tle_size);
            if (success) {
                pb_istream_t stream = pb_istream_from_buffer(buffer, Tle_size);
                Tle tle = Tle_init_zero;
                bool status = pb_decode(&stream, Tle_fields, &tle);
                if (!status) {
                    etl::exception e("pb_decode failed", __FILE__, __LINE__);
                    throw e;
                }

                location_estimator.StoreTle(tle);

                // Calculate position
                // TODO (rskew) calculate absolute time in millis since tle
                // epoch
                tsince_millis += 50 * 100;
                location_estimator.UpdateLocation(tsince_millis);

                // Write calculated position to data dashboard
                LocationReading location_reading = LocationReading_init_zero;
                location_reading.lattitude_geodetic_degrees =
                    location_estimator.GetLattitudeGeodeticDegrees();
                location_reading.longitude_degrees =
                    location_estimator.GetLongitudeDegrees();
                location_reading.altitude_above_ellipsoid_km =
                    location_estimator.GetAltitudeAboveEllipsoidKm();
                // TODO (rskew) generate timestamp
                location_reading.timestamp_millis_unix_epoch = 0;
                RunnableDataDashboard::TransmitMessage(
                    kLocationReadingCode, LocationReading_size,
                    LocationReading_fields, &location_reading);
            }
        }
    }
}
