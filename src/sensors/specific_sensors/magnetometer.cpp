#include <Board.h>
#include <external/etl/exception.h>
#include <external/nanopb/pb_decode.h>
#include <external/nanopb/pb_encode.h>
#include <src/adcs/controllers/b_dot_controller.h>
#include <src/adcs/state_estimators/b_dot_estimator.h>
#include <src/config/unit_tests.h>
#include <src/data_dashboard/runnable_data_dashboard.h>
#include <src/debug_interface/debug_stream.h>
#include <src/messages/MagnetometerReading.pb.h>
#include <src/messages/TorqueOutputReading.pb.h>
#include <src/sensors/generic_measurable.h>
#include <src/sensors/specific_sensors/magnetometer.h>
#include <src/util/message_codes.h>
#include <ti/sysbios/BIOS.h>
#include <ti/sysbios/knl/Semaphore.h>

const char Magnetometer::kCalibrationReadingsBufferAFileName[] = "magcal_a.bin";
const char Magnetometer::kCalibrationReadingsBufferBFileName[] = "magcal_b.bin";

Magnetometer::Magnetometer()
    : calibration_readings_buffer_a(kCalibrationReadingsBufferSize,
                                    kCalibrationReadingsBufferAFileName),
      calibration_readings_buffer_b(kCalibrationReadingsBufferSize,
                                    kCalibrationReadingsBufferBFileName),
      // Direct bus access to be replaced with I2cMeasureableManager
      imu_a(new I2c(I2C_BUS_A), kImuAddress, "imu_a"),
      imu_b(new I2c(I2C_BUS_B), kImuAddress, "imu_b") {
    imu_a.SetMagnetometerOperationMode(kMagnoSingleMeasurement);
    imu_b.SetMagnetometerOperationMode(kMagnoSingleMeasurement);
    imu_a.SetMagnetometerOutputBitSetting(k16BitOutput);
    imu_b.SetMagnetometerOutputBitSetting(k16BitOutput);
};

// Get readings from the hardware magnetometer and the simulation.
// Use the simulation readings for the controller, but echo the hardware
// readings to the DebugClient to show that it's working.
//////////////////////////////////////////////////
// For hardware tests, make sure to store the hardware readings
// for calibration.
// For HIL tests, store the simulation readings for calibration.
bool Magnetometer::TakeReading() {
    MagnetometerReading imu_a_reading;
    MagnetometerReading imu_b_reading;
    try {
        imu_a.TakeMagnetometerReading(imu_a_reading);
    } catch (etl::exception e) {
        imu_a_reading.x = 0;
        imu_a_reading.y = 0;
        imu_a_reading.z = 0;
    }
    try {
        imu_b.TakeMagnetometerReading(imu_b_reading);
    } catch (etl::exception e) {
        imu_b_reading.x = 0;
        imu_b_reading.y = 0;
        imu_b_reading.z = 0;
    }
    if (hil_enabled) {
        // Echo reading to data dashboard
        RunnableDataDashboard::TransmitMessage(
            kMagnetometerReadingCode, MagnetometerReading_size,
            MagnetometerReading_fields, &imu_a_reading);
        RunnableDataDashboard::TransmitMessage(
            kMagnetometerReadingCode, MagnetometerReading_size,
            MagnetometerReading_fields, &imu_b_reading);
    }

    try {
    
        calibration_readings_buffer_b.WriteMessage(imu_b_reading);
    } catch (etl::exception e) {
        // TODO (rskew) catch exceptions for pb encode error, sdcard write
        // error,
        throw e;
    }

    // TODO (rskew) apply calibration to readings

    // Average readings from IMUs
    reading.x = (imu_a_reading.x + imu_b_reading.x) / 2;
    reading.y = (imu_a_reading.y + imu_b_reading.y) / 2;
    reading.z = (imu_a_reading.z + imu_b_reading.z) / 2;

    // TODO (rskew) combine noise from hardware readings with simulated readings

    // HIL reading is returned for hardware tests so that controller can
    // activate magnetorquers in response to simulated tumbling
    bool success;
    if (hil_enabled) {
        success = TakeReadingHil();
    }

    return success;
}

bool Magnetometer::TakeReadingHil() {
    DebugStream *debug_stream = DebugStream::GetInstance();
    uint8_t buffer[MagnetometerReading_size];

    bool success = debug_stream->RequestMessageFromSimulator(
        kMagnetometerReadingRequestCode, buffer, MagnetometerReading_size);

    if (success) {
        pb_istream_t stream =
            pb_istream_from_buffer(buffer, MagnetometerReading_size);

        bool status = pb_decode(&stream, MagnetometerReading_fields, &reading);
        if (!status) {
            etl::exception e("pb_decode failed", __FILE__, __LINE__);
            throw e;
        }

        NotifyObservers();
    }

    return success;
}
