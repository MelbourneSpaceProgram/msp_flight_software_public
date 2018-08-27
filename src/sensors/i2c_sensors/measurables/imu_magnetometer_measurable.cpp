#include <external/nanopb/pb_decode.h>
#include <external/nanopb/pb_encode.h>
#include <src/board/debug_interface/debug_stream.h>
#include <src/config/unit_tests.h>
#include <src/data_dashboard/runnable_data_dashboard.h>
#include <src/messages/MagnetometerReading.pb.h>
#include <src/sensors/i2c_sensors/measurables/imu_magnetometer_measurable.h>
#include <src/util/message_codes.h>

const MagnetometerReading
    ImuMagnetometerMeasurable::kFailedMagnetometerReading = {-9999, -9999,
                                                             -9999, 0};

ImuMagnetometerMeasurable::ImuMagnetometerMeasurable(
    MPU9250MotionTracker* imu_sensor, const Matrix frame_mapping)
    : I2cMeasurable<MagnetometerReading>(imu_sensor,
                                         kFailedMagnetometerReading),
      magnetometer_to_body_frame_transform(frame_mapping) {}

// Get readings from the hardware magnetometer and the simulation.
// Fuse the hardware and simulation readings for the controller, and
// echo readings to the DebugClient.
MagnetometerReading ImuMagnetometerMeasurable::TakeDirectI2cReading() {
    MPU9250MotionTracker* imu_sensor =
        static_cast<MPU9250MotionTracker*>(I2cMeasurable::sensor);
    MagnetometerReading raw_reading = imu_sensor->TakeMagnetometerReading();

    // Apply change of frame from magnetometer frame to satellite body frame
    double reading_magnetometer_frame_data[3][1];
    Matrix reading_magnetometer_frame(reading_magnetometer_frame_data);
    reading_magnetometer_frame.Set(0, 0, raw_reading.x);
    reading_magnetometer_frame.Set(1, 0, raw_reading.y);
    reading_magnetometer_frame.Set(2, 0, raw_reading.z);
    double reading_body_frame_data[3][1];
    Matrix reading_body_frame(reading_body_frame_data);
    reading_body_frame.Multiply(magnetometer_to_body_frame_transform,
                                reading_magnetometer_frame);
    reading.x = reading_body_frame.Get(0,0);
    reading.y = reading_body_frame.Get(1,0);
    reading.z = reading_body_frame.Get(2,0);


    if (hil_available) {
        // Echo reading to data dashboard
        RunnableDataDashboard::TransmitMessage(
            kMagnetometerReadingCode, MagnetometerReading_size,
            MagnetometerReading_fields, &reading);
    }

    if (hil_available) {
        MagnetometerReading simulation_reading = TakeSimulationReading();
        // Combine readings.
        // The static hardware reading will be calibrated out, and the
        // true hardware noise will be added to the simulated reading.
        reading.x = reading.x + simulation_reading.x;
        reading.y = reading.y + simulation_reading.y;
        reading.z = reading.z + simulation_reading.z;
    }

    // TODO (rskew) write to circular buffer for calibration

    // TODO (rskew) apply calibration operations

    return reading;
}

MagnetometerReading ImuMagnetometerMeasurable::TakeSimulationReading() {
    DebugStream* debug_stream = DebugStream::GetInstance();
    uint8_t buffer[MagnetometerReading_size];
    MagnetometerReading simulation_reading;

    bool success = debug_stream->RequestMessageFromSimulator(
        kMagnetometerReadingRequestCode, buffer, MagnetometerReading_size);

    if (success) {
        pb_istream_t stream =
            pb_istream_from_buffer(buffer, MagnetometerReading_size);

        bool status =
            pb_decode(&stream, MagnetometerReading_fields, &simulation_reading);
        if (!status) {
            etl::exception e("pb_decode failed", __FILE__, __LINE__);
            throw e;
        }
    } else {
        etl::exception e("Failed to request message from simulator", __FILE__,
                         __LINE__);
        throw e;
    }

    return simulation_reading;
}
