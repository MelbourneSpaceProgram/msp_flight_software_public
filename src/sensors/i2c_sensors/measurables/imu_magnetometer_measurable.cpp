#include <external/nanopb/pb_decode.h>
#include <external/nanopb/pb_encode.h>
#include <src/board/debug_interface/debug_stream.h>
#include <src/config/unit_tests.h>
#include <src/data_dashboard/runnable_data_dashboard.h>
#include <src/database/circular_buffer_nanopb.h>
#include <src/messages/MagnetometerReading.pb.h>
#include <src/sensors/i2c_sensors/measurables/imu_magnetometer_measurable.h>
#include <src/util/etl_utils.h>
#include <src/util/message_codes.h>

const char* ImuMagnetometerMeasurable::kCalibrationReadingsBufferFileName =
    "magcal.pb";

const MagnetometerReading
    ImuMagnetometerMeasurable::kFailedMagnetometerReading = {-9999, -9999,
                                                             -9999, 0};

ImuMagnetometerMeasurable::ImuMagnetometerMeasurable(
    MPU9250MotionTracker* imu_sensor, const Matrix& frame_mapping,
    const Matrix& initial_biases, const Matrix& initial_scale_factors)
    : I2cMeasurable<MagnetometerReading>(imu_sensor,
                                         kFailedMagnetometerReading),
      magnetometer_to_body_frame_transform(frame_mapping),
      magnetometer_calibration(initial_biases, initial_scale_factors),
      can_calibrate(true) {
    if (kSdCardAvailable) {
        try {
            CircularBufferNanopb(MagnetometerReading)::Create(
                kCalibrationReadingsBufferFileName,
                kCalibrationReadingsBufferSizeInReadings);
        } catch (etl::exception& e) {
            EtlUtils::LogException(e);
            Log_error0(
                "Disabling magnetometer calibration due to Circular Buffer/SD "
                "error");
            can_calibrate = false;
        }
    } else {
        Log_info0("Disabling magnetometer calibration due to no SD card");
        can_calibrate = false;
    }
}

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
    reading.x = reading_body_frame.Get(0, 0);
    reading.y = reading_body_frame.Get(1, 0);
    reading.z = reading_body_frame.Get(2, 0);

    if (kHilAvailable) {
        // Echo reading to data dashboard
        RunnableDataDashboard::TransmitMessage(
            kMagnetometerReadingCode, MagnetometerReading_size,
            MagnetometerReading_fields, &reading);
    }

    if (kHilAvailable) {
        MagnetometerReading simulation_reading = TakeSimulationReading();
        // Combine readings.
        // The static hardware reading will be calibrated out, and the
        // true hardware noise will be added to the simulated reading.
        reading.x = reading.x + simulation_reading.x;
        reading.y = reading.y + simulation_reading.y;
        reading.z = reading.z + simulation_reading.z;
    }

    if (can_calibrate) {
        // Write to circular buffer for calibration.
        // TODO (rskew) catch exceptions for pb encode error, sdcard write
        // error,
        CircularBufferNanopb(MagnetometerReading)::WriteMessage(
            kCalibrationReadingsBufferFileName, reading);

        // Apply calibration operations
        magnetometer_calibration.Apply(reading);
    } else {
        Log_info0(
            "Skipping magnetometer calibration due to error or no SD card");
    }

    if (kHilAvailable) {
        RunnableDataDashboard::TransmitMessage(
            kCalibratedMagnetometerReadingCode, MagnetometerReading_size,
            MagnetometerReading_fields, &reading);
    }

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

bool ImuMagnetometerMeasurable::Calibrate() {
    if (!kSdCardAvailable) {
        return false;
    }

    // If the buffer doesn't have enough readings the
    // calibration could be invalid
    if (CircularBufferNanopb(MagnetometerReading)::ReadCountMessagesWritten(
            kCalibrationReadingsBufferFileName) <
        MagnetometerCalibration::kMinimumSamplesForValidCalibration) {
        Log_info0(
            "Not enough samples inthe circular buffer to calibrate the "
            "magnetometer");
        return false;
    }

    for (uint32_t i = 0; i < kCalibrationReadingsBufferSizeInReadings /
                                 MagnetometerCalibration::kBatchSizeInReadings;
         i++) {
        // read data from circular buffer
        MagnetometerReading magnetometer_readings_batch
            [MagnetometerCalibration::kBatchSizeInReadings];
        for (uint8_t j = 0; j < MagnetometerCalibration::kBatchSizeInReadings;
             j++) {
            try {
                magnetometer_readings_batch[j] =
                    GetReadingFromBuffer(kCalibrationReadingsBufferFileName);
            } catch (etl::exception& e) {
                Log_info0(
                    "Unable to read from magnetometer calibration circular "
                    "buffer");
                return false;
            }
        }

        double magnetometer_readings_batch_matrix_data
            [MagnetometerCalibration::kBatchSizeInReadings][3];
        Matrix magnetometer_readings_batch_matrix(
            magnetometer_readings_batch_matrix_data);
        for (uint16_t i = 0; i < MagnetometerCalibration::kBatchSizeInReadings;
             i++) {
            magnetometer_readings_batch_matrix.Set(
                i, 0, magnetometer_readings_batch[i].x);
            magnetometer_readings_batch_matrix.Set(
                i, 1, magnetometer_readings_batch[i].y);
            magnetometer_readings_batch_matrix.Set(
                i, 2, magnetometer_readings_batch[i].z);
        }
        magnetometer_calibration.AggregateReadings(
            magnetometer_readings_batch_matrix);
    }

    magnetometer_calibration.ComputeCalibrationParameters();

    return true;
}

MagnetometerReading ImuMagnetometerMeasurable::GetReadingFromBuffer(
    const char* file_name) {
    // Try to read from the buffer a number of times before giving up
    for (int i = 0; i < ImuMagnetometerMeasurable::kBufferReadAttempts; i++) {
        try {
            return CircularBufferNanopb(MagnetometerReading)::ReadMessage(
                file_name);
        } catch (etl::exception& e) {
            // If the Hamming decoding fails, discard the
            // message and read another one.
            continue;
            // Note: if too many readings fail, the calibration could
            // not have enough samples and be invalid.
            // This is considered unlikely.
        }
    }
    etl::exception e("Unable to read valid data from buffer", __FILE__,
                     __LINE__);
    throw e;
}
