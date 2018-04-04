#include <src/adcs/controllers/b_dot_controller.h>
#include <src/adcs/runnable_orientation_control.h>
#include <src/adcs/state_estimators/b_dot_estimator.h>
#include <src/data_dashboard/runnable_data_dashboard.h>
#include <src/debug_interface/debug_stream.h>
#include <src/init/init.h>
#include <src/messages/MagnetometerReading.pb.h>
#include <src/messages/TorqueOutputReading.pb.h>
#include <src/sensors/specific_sensors/magnetometer.h>
#include <src/util/message_codes.h>
#include <ti/sysbios/BIOS.h>
#include <ti/sysbios/knl/Semaphore.h>

RunnableOrientationControl::RunnableOrientationControl() {}

fnptr RunnableOrientationControl::GetRunnablePointer() {
    return &RunnableOrientationControl::ControlOrientation;
}

void RunnableOrientationControl::ControlOrientation() {
    DebugStream* debug_stream = DebugStream::GetInstance();
    Magnetometer magnetometer("this id to be replaced with signal_code");
    BDotEstimator b_dot_estimator(50, 4000);

    while (1) {
        // TODO(rskew) switch algorithms based on AdcsStateMachine state

        // Read Magnetometer
        // TODO(rskew) handle false return value
        bool success = magnetometer.TakeReading();
        MagnetometerReading reading = magnetometer.GetReading();

        // Echo reading to data dashboard
        RunnableDataDashboard::TransmitMessage(
            kMagnetometerReadingCode, MagnetometerReading_size,
            MagnetometerReading_fields, &reading);

        // Run estimator
        double magnetometer_reading_data[3][1] = {
            {reading.x}, {reading.y}, {reading.z}};
        Matrix magnetometer_reading(magnetometer_reading_data);
        double b_dot_estimate_data[3][1];
        Matrix b_dot_estimate(b_dot_estimate_data);
        b_dot_estimator.Estimate(magnetometer_reading, b_dot_estimate);

        // TODO(rskew) tell DetumbledStateMachine about Bdot (or omega?)

        // Run controller
        double torque_output_data[3][1];
        Matrix torque_output(torque_output_data);
        BDotController::Control(magnetometer_reading, b_dot_estimate,
                                torque_output);

        // Send torque output to simulation
        TorqueOutputReading torque_output_reading =
            TorqueOutputReading_init_zero;
        torque_output_reading.x = torque_output.Get(0, 0);
        torque_output_reading.y = torque_output.Get(1, 0);
        torque_output_reading.z = torque_output.Get(2, 0);

        RunnableDataDashboard::TransmitMessage(
            kTorqueOutputReadingCode, TorqueOutputReading_size,
            TorqueOutputReading_fields, &torque_output_reading);

        // TODO(rskew): allow multiple tasks to write to debug stream
        // so this doesn't have to be here
        RunnableDataDashboard::DataDashboard();

        // TODO (rskew) use a hard real time timer to trigger this instead of
        // sleep
        Task_sleep(50);
    }
}
