#include <src/board/i2c/i2c.h>
#include <src/config/satellite.h>
#include <src/messages/AccelerometerReading.pb.h>
#include <src/messages/GyroscopeReading.pb.h>
#include <src/sensors/i2c_sensors/mpu9250_motion_tracker.h>
#include <src/util/msp_exception.h>
#include <ti/sysbios/knl/Task.h>

double kImuAToBodyFrameTransform_dummy_data[3][3];
double kImuBToBodyFrameTransform_dummy_data[3][3];
double kBodyToMagnetorquerFrameTransform_dummy_data[3][3];

const uint16_t MPU9250MotionTracker::kGyroscopeFullScaleRanges[4] = {
    250, 500, 1000, 2000};

const float MPU9250MotionTracker::kGyroscopeSensitivityScaleFactors[4] = {
    131, 65.5, 32.8, 16.4};

const byte MPU9250MotionTracker::kAccelerometerFullScaleRanges[4] = {2, 4, 8,
                                                                     16};

const uint16_t MPU9250MotionTracker::kAccelerometerSensitivityScaleFactors[4] =
    {16384, 8192, 4096, 2048};

MPU9250MotionTracker::MPU9250MotionTracker(const I2c* bus, uint8_t address,
                                           const I2cMultiplexer* multiplexer,
                                           I2cMultiplexer::MuxChannel channel)
    : I2cDevice(bus, address, multiplexer, channel) {
    if (!kFsBoardAvailable) {
        return;
    }

    try {
        InitialiseImu();
    } catch (MspException& e) {
        MspException::LogException(e, kImuCatch);
    }
}

void MPU9250MotionTracker::InitialiseImu() {
    // default settings for the gyroscope/accelerometer
    SetGyroFullScaleSetting(kGyro250dps);
    SetAccelFullScaleSetting(kAccel2g);
    // default settings for the magnetometer
    SetBypassMode(kBypassModeEnable);
    SetMagnetometerOperationMode(kMagnoContinuousMeasurement1);
    SetMagnetometerOutputBitSetting(k14BitOutput);
    SelectMagnetometerRegister(kMagnoAdjustX);
    ReadMagnetometerAdjustmentValues();
    SetBypassMode(kBypassModeDisable);
}

GyroscopeReading MPU9250MotionTracker::TakeGyroscopeReading() {
    GyroscopeReading gyroscope_reading = GyroscopeReading_init_default;
    SelectRegister(kGyroXOutHigh);

    etl::array<byte, 6> gyro_reading_bytes = ReadSixBytesFromCurrentRegister();
    etl::array<byte, 2> gyro_x_reading_bytes, gyro_y_reading_bytes,
        gyro_z_reading_bytes;
    gyro_x_reading_bytes[0] = gyro_reading_bytes[0];
    gyro_x_reading_bytes[1] = gyro_reading_bytes[1];
    gyro_y_reading_bytes[0] = gyro_reading_bytes[2];
    gyro_y_reading_bytes[1] = gyro_reading_bytes[3];
    gyro_z_reading_bytes[0] = gyro_reading_bytes[4];
    gyro_z_reading_bytes[1] = gyro_reading_bytes[5];

    gyroscope_reading.x = DecodeGyroReadingToSI(gyro_x_reading_bytes);
    gyroscope_reading.y = DecodeGyroReadingToSI(gyro_y_reading_bytes);
    gyroscope_reading.z = DecodeGyroReadingToSI(gyro_z_reading_bytes);

    return gyroscope_reading;
}

AccelerometerReading MPU9250MotionTracker::TakeAccelerometerReading() {
    AccelerometerReading accelerometer_reading =
        AccelerometerReading_init_default;
    SelectRegister(kAccelXOutHigh);

    etl::array<byte, 6> accel_reading_bytes = ReadSixBytesFromCurrentRegister();
    etl::array<byte, 2> accel_x_reading_bytes, accel_y_reading_bytes,
        accel_z_reading_bytes;
    accel_x_reading_bytes[0] = accel_reading_bytes[0];
    accel_x_reading_bytes[1] = accel_reading_bytes[1];
    accel_y_reading_bytes[0] = accel_reading_bytes[2];
    accel_y_reading_bytes[1] = accel_reading_bytes[3];
    accel_z_reading_bytes[0] = accel_reading_bytes[4];
    accel_z_reading_bytes[1] = accel_reading_bytes[5];

    accelerometer_reading.x = DecodeAccelReadingToSI(accel_x_reading_bytes);
    accelerometer_reading.y = DecodeAccelReadingToSI(accel_y_reading_bytes);
    accelerometer_reading.z = DecodeAccelReadingToSI(accel_z_reading_bytes);

    return accelerometer_reading;
}

double MPU9250MotionTracker::TakeTemperatureReading() {
    double temperature_reading;
    SelectRegister(kTempOutHigh);
    etl::array<byte, 2> temp_reading_bytes = ReadTwoBytesFromCurrentRegister();
    temperature_reading = DecodeTempReadingToSI(temp_reading_bytes);

    return temperature_reading;
}

MagnetometerReading MPU9250MotionTracker::TakeMagnetometerReading() {
    MagnetometerReading magnetometer_reading = MagnetometerReading_init_default;
    SetBypassMode(kBypassModeEnable);
    SelectMagnetometerRegister(kExtMagnoXOutHigh);

    etl::array<byte, 2> magno_x_reading_bytes, magno_y_reading_bytes,
        magno_z_reading_bytes;
    etl::array<byte, 7> magno_data = ReadSevenBytesFromMagnoRegister();

    if (magno_data.at(6) & kMagnetometerOverflowBitMask) {
        throw MspException("Magnetometer overflow",
                           kImuMagnetometerOverflowFail, __FILE__, __LINE__);
    }

    // data is stored in Little Endian format
    magno_x_reading_bytes[0] = magno_data[1];
    magno_x_reading_bytes[1] = magno_data[0];
    magno_y_reading_bytes[0] = magno_data[3];
    magno_y_reading_bytes[1] = magno_data[2];
    magno_z_reading_bytes[0] = magno_data[5];
    magno_z_reading_bytes[1] = magno_data[4];

    // adjust the magnetometer readings
    magnetometer_reading.x =
        DecodeMagnoReadingToSI(magno_x_reading_bytes, magno_x_adjust);
    magnetometer_reading.y =
        DecodeMagnoReadingToSI(magno_y_reading_bytes, magno_y_adjust);
    magnetometer_reading.z =
        DecodeMagnoReadingToSI(magno_z_reading_bytes, magno_z_adjust);

    SetBypassMode(kBypassModeDisable);

    return magnetometer_reading;
}

void MPU9250MotionTracker::ReadMagnetometerAdjustmentValues() {
    // access fuse ROM registers
    SetMagnetometerOperationMode(kMagnoPowerDown);
    Task_sleep(100);  // required after entering power down mode
    SetMagnetometerOperationMode(kMagnoFuseRomAccess);
    Task_sleep(100);

    // read fuse ROM registers
    byte magno_data_adjust_values[3];
    bool magno_data_adjust_values_read = PerformReadTransaction(
        kInternalMagnetometerAddress, magno_data_adjust_values, 3);

    if (magno_data_adjust_values_read) {
        magno_x_adjust = magno_data_adjust_values[0];
        magno_y_adjust = magno_data_adjust_values[1];
        magno_z_adjust = magno_data_adjust_values[2];
    } else {
        throw MspException("Unable to read fuse ROM registers",
                           kImuFuseRomRegisterReadFail, __FILE__, __LINE__);
    }

    // revert to continuous measurement mode
    SetMagnetometerOperationMode(kMagnoPowerDown);
    Task_sleep(100);
    SetMagnetometerOperationMode(kMagnoContinuousMeasurement1);
    Task_sleep(100);
}

void MPU9250MotionTracker::SelectRegister(byte register_address) {
    PerformWriteTransaction(address, &register_address, 1);
}

etl::array<byte, 7> MPU9250MotionTracker::ReadSevenBytesFromMagnoRegister() {
    byte i2c_buffer[7];
    etl::array<byte, 7> read_buffer;
    if (PerformReadTransaction(kInternalMagnetometerAddress, i2c_buffer, 7)) {
        uint16_t i;
        for (i = 0; i < 7; i++) {
            read_buffer.at(i) = i2c_buffer[i];
        }
    } else {
        throw MspException("Unable to read from register", kImuRegisterReadFail,
                           __FILE__, __LINE__);
    }
    return read_buffer;
}

etl::array<byte, 6> MPU9250MotionTracker::ReadSixBytesFromCurrentRegister() {
    byte i2c_buffer[6];
    etl::array<byte, 6> read_buffer;
    if (PerformReadTransaction(address, i2c_buffer, 6)) {
        uint16_t i;
        for (i = 0; i < 6; i++) {
            read_buffer[i] = i2c_buffer[i];
        }
    } else {
        throw MspException("Unable to read from register",
                           kImuRegisterRead2Fail, __FILE__, __LINE__);
    }
    return read_buffer;
}

etl::array<byte, 2> MPU9250MotionTracker::ReadTwoBytesFromCurrentRegister() {
    byte i2c_buffer[2];
    etl::array<byte, 2> read_buffer;
    if (PerformReadTransaction(address, i2c_buffer, 2)) {
        uint16_t i;
        for (i = 0; i < 2; i++) {
            read_buffer[i] = i2c_buffer[i];
        }
    } else {
        throw MspException("Unable to read from register",
                           kImuRegisterRead3Fail, __FILE__, __LINE__);
    }
    return read_buffer;
}

double MPU9250MotionTracker::DecodeGyroReadingToSI(
    etl::array<byte, 2> two_byte_gyro_reading) {
    int16_t binary_gyro_reading =
        ConvertTwoByteReadingToBinaryReading(two_byte_gyro_reading);
    return ConvertBinaryGyroReadingToSI(binary_gyro_reading);
}

double MPU9250MotionTracker::DecodeAccelReadingToSI(
    etl::array<byte, 2> two_byte_accel_reading) {
    int16_t binary_accel_reading =
        ConvertTwoByteReadingToBinaryReading(two_byte_accel_reading);
    return ConvertBinaryAccelReadingToSI(binary_accel_reading);
}

double MPU9250MotionTracker::DecodeTempReadingToSI(
    etl::array<byte, 2> two_byte_temp_reading) {
    int16_t binary_temp_reading =
        ConvertTwoByteReadingToBinaryReading(two_byte_temp_reading);
    return ConvertBinaryTempReadingToSI(binary_temp_reading);
}

double MPU9250MotionTracker::DecodeMagnoReadingToSI(
    etl::array<byte, 2> two_byte_magno_reading, byte magno_adjust_value) {
    int16_t binary_magno_reading;
    binary_magno_reading =
        ConvertTwoByteReadingToBinaryReading(two_byte_magno_reading);
    return ConvertBinaryMagnoReadingToSI(binary_magno_reading,
                                         magno_adjust_value);
}

int16_t MPU9250MotionTracker::ConvertTwoByteReadingToBinaryReading(
    etl::array<byte, 2> two_byte_array) {
    int16_t binary_reading = ((static_cast<int16_t>(two_byte_array[0])) << 8) |
                             static_cast<int16_t>(two_byte_array[1]);
    return binary_reading;
}

double MPU9250MotionTracker::ConvertBinaryGyroReadingToSI(
    int16_t binary_reading) {
    return 1.0 * binary_reading / gyro_sensitivity_scale_factor;
}

double MPU9250MotionTracker::ConvertBinaryAccelReadingToSI(
    int16_t binary_reading) {
    return 1.0 * binary_reading / accel_sensitivity_scale_factor;
}

double MPU9250MotionTracker::ConvertBinaryTempReadingToSI(
    int16_t binary_reading) {
    return ((binary_reading - kRoomTempOffset) / kTempSensitivity) + 21;
}

double MPU9250MotionTracker::ConvertBinaryMagnoReadingToSI(
    int16_t magno_reading, byte magno_adjust_value) {
    double scaled_magno_reading, adjusted_magno_reading;
    scaled_magno_reading = 1.0 * magno_reading * kMaxMeasurableFluxDensity /
                           magnetometer_measurement_range;
    adjusted_magno_reading =
        scaled_magno_reading *
        ((((magno_adjust_value - 128) >> 1) / 128) + 1);  // datasheet formula
    return adjusted_magno_reading;
}

void MPU9250MotionTracker::SetGyroConfiguration() {
    byte package[2];
    package[0] = kGyroConfigRegister;
    package[1] = (gyro_full_scale_setting << 4);
    PerformWriteTransaction(address, package, 2);
}

void MPU9250MotionTracker::SetAccelConfiguration() {
    byte package[2];
    package[0] = kAccelConfigRegister1;
    package[1] = (accel_full_scale_setting << 4);
    PerformWriteTransaction(address, package, 2);
}

void MPU9250MotionTracker::SetGyroFullScaleSetting(
    GyroFullScaleValue gyro_full_scale_setting) {
    this->gyro_full_scale_setting = gyro_full_scale_setting;
    this->gyro_full_scale_range = MPU9250MotionTracker::
        kGyroscopeFullScaleRanges[gyro_full_scale_setting];
    this->gyro_sensitivity_scale_factor = MPU9250MotionTracker::
        kGyroscopeSensitivityScaleFactors[gyro_full_scale_setting];
    SetGyroConfiguration();
}

void MPU9250MotionTracker::SetAccelFullScaleSetting(
    AccelFullScaleValue accel_full_scale_setting) {
    this->accel_full_scale_setting = accel_full_scale_setting;
    this->accel_full_scale_range = MPU9250MotionTracker::
        kAccelerometerFullScaleRanges[accel_full_scale_setting];
    this->accel_sensitivity_scale_factor = MPU9250MotionTracker::
        kAccelerometerSensitivityScaleFactors[accel_full_scale_setting];
    SetAccelConfiguration();
}

void MPU9250MotionTracker::SetBypassMode(BypassMode bypass_mode) {
    byte package[2];
    package[0] = kBypassEnable;
    package[1] = (bypass_mode << 1);
    PerformWriteTransaction(address, package, 2);
}

void MPU9250MotionTracker::SelectMagnetometerRegister(
    byte magnetometer_register_address) {
    PerformWriteTransaction(kInternalMagnetometerAddress,
                            &magnetometer_register_address, 1);
}

void MPU9250MotionTracker::SetMagnetometerOperationMode(
    MagnetometerOperationMode magnetometer_operation_mode) {
    this->magnetometer_operation_mode = magnetometer_operation_mode;
    ConfigureMagnetometer();
}

void MPU9250MotionTracker::SetMagnetometerOutputBitSetting(
    MagnetometerOutputBitSetting magnetometer_output_bit_setting) {
    this->magnetometer_output_bit_setting = magnetometer_output_bit_setting;

    if (magnetometer_output_bit_setting == k14BitOutput) {
        magnetometer_measurement_range = k14BitOutputMeasurementRange;
    } else if (magnetometer_output_bit_setting == k16BitOutput) {
        magnetometer_measurement_range = k16BitOutputMeasurementRange;
    }
    ConfigureMagnetometer();
}

void MPU9250MotionTracker::ConfigureMagnetometer() {
    byte package[2];
    package[0] = kMagnetometerControlRegister;
    package[1] =
        (magnetometer_output_bit_setting << 4) | magnetometer_operation_mode;
    PerformWriteTransaction(kInternalMagnetometerAddress, package, 2);
}
