#ifndef SRC_SENSORS_I2C_SENSORS_MPU9250_MOTION_TRACKER_H
#define SRC_SENSORS_I2C_SENSORS_MPU9250_MOTION_TRACKER_H

#include <external/etl/array.h>
#include <external/etl/exception.h>
#include <src/messages/AccelerometerReading.pb.h>
#include <src/messages/GyroscopeReading.pb.h>
#include <src/messages/MagnetometerReading.pb.h>
#include <src/sensors/i2c_sensors/i2c_sensor.h>
#include <src/util/data_types.h>

class I2c;

// data types for the gyroscope/accelerometer chip
enum GyroFullScaleValue {
    kGyro250dps = 0x00,
    kGyro500dps = 0x01,
    kGyro1000dps = 0x02,
    kGyro2000dps = 0x03
};

enum AccelFullScaleValue {
    kAccel2g = 0x00,
    kAccel4g = 0x01,
    kAccel8g = 0x02,
    kAccel16g = 0x03
};

enum BypassMode { kBypassModeDisable = 0, kBypassModeEnable = 1 };

// data types for the magnetometer chip
enum MagnetometerOperationMode {
    kMagnoPowerDown = 0x00,
    kMagnoSingleMeasurement = 0x01,
    kMagnoContinuousMeasurement1 = 0x02,
    kMagnoContinuousMeasurement2 = 0x03,
    kMagnoExtTrigMeasurement = 0x04,
    kMagnoSelfTest = 0x08,
    kMagnoFuseRomAccess = 0x0F
};

enum MagnetometerOutputBitSetting { k14BitOutput = 0, k16BitOutput = 1 };

class MPU9250MotionTracker : public I2cSensor {
   public:
    MPU9250MotionTracker(
        const I2c* bus, int address, const I2cMultiplexer* multiplexer = NULL,
        I2cMultiplexer::MuxChannel channel = I2cMultiplexer::kMuxNoChannel);
    GyroscopeReading TakeGyroscopeReading();
    AccelerometerReading TakeAccelerometerReading();
    double TakeTemperatureReading();
    MagnetometerReading TakeMagnetometerReading();

    void SetGyroFullScaleSetting(GyroFullScaleValue gyro_full_scale_value);
    void SetAccelFullScaleSetting(AccelFullScaleValue accel_full_scale_value);
    void SetMagnetometerOperationMode(
        MagnetometerOperationMode magnetometer_operation_mode);
    void SetMagnetometerOutputBitSetting(
        MagnetometerOutputBitSetting magnetometer_output_bit_setting);

   private:
    // gyroscope configuration
    void SetGyroConfiguration();
    byte gyro_full_scale_setting;
    uint16_t gyro_full_scale_range;
    float gyro_sensitivity_scale_factor;
    static const uint16_t kGyroscopeFullScaleRanges[4];
    static const float kGyroscopeSensitivityScaleFactors[4];

    // accelerometer configuration
    void SetAccelConfiguration();
    byte accel_full_scale_setting;
    byte accel_full_scale_range;
    uint16_t accel_sensitivity_scale_factor;
    static const byte kAccelerometerFullScaleRanges[4];
    static const uint16_t kAccelerometerSensitivityScaleFactors[4];

    // methods for reading from and writing to the MPU9250
    void SelectRegister(byte register_address);
    etl::array<byte, 2> ReadTwoBytesFromCurrentRegister();
    etl::array<byte, 6> ReadSixBytesFromCurrentRegister();

    // method to allow i2c communication to magnetometer
    void SetBypassMode(BypassMode bypass_mode);

    // methods and variables for reading from the magnetometer
    void ConfigureMagnetometer();
    void SelectMagnetometerRegister(byte magnetometer_register_address);
    void ReadMagnetometerAdjustmentValues();
    MagnetometerOperationMode magnetometer_operation_mode;
    MagnetometerOutputBitSetting magnetometer_output_bit_setting;
    byte magno_x_adjust;
    byte magno_y_adjust;
    byte magno_z_adjust;
    int16_t magnetometer_measurement_range;
    static const int16_t k14BitOutputMeasurementRange = 8190;
    static const int16_t k16BitOutputMeasurementRange = 32760;
    static const int16_t kMaxMeasurableFluxDensity = 4912;

    // methods for converting byte readings to values in SI units
    double DecodeGyroReadingToSI(etl::array<byte, 2> two_byte_gyro_reading);
    double DecodeAccelReadingToSI(etl::array<byte, 2> two_byte_accel_reading);
    double DecodeTempReadingToSI(etl::array<byte, 2> two_byte_temp_reading);
    double DecodeMagnoReadingToSI(etl::array<byte, 2> two_byte_magno_reading,
                                  byte magno_adjust_value);
    int16_t ConvertTwoByteReadingToBinaryReading(
        etl::array<byte, 2> two_byte_array);
    double ConvertBinaryGyroReadingToSI(int16_t binary_reading);
    double ConvertBinaryAccelReadingToSI(int16_t binary_reading);
    double ConvertBinaryTempReadingToSI(int16_t binary_reading);
    double ConvertBinaryMagnoReadingToSI(int16_t magno_reading,
                                         byte magno_adjust_value);

    // temperature sensor constants from data sheet
    static const int16_t kMaxOperatingTemp = 85;
    static const int16_t kMinOperatingTemp = -40;
    static const double kTempSensitivity = 333.87;  // LSBs per degree Celsius
    static const byte kRoomTempOffset = 21;

    static const byte kInternalMagnetometerAddress = 12;

    // register locations of the gyroscope and accelerometer configuration
    static const byte kGyroConfigRegister = 0x1b;
    static const byte kAccelConfigRegister1 = 0x1c;
    static const byte kAccelConfigRegister2 = 0x1d;

    // register location of the bypass enable register
    static const uint8_t kBypassEnable = 0x37;

    // register locations of accelerometer x, y, z readings
    static const uint8_t kAccelXOutHigh = 0x3b;
    static const uint8_t kAccelXOutLow = 0x3c;
    static const uint8_t kAccelYOutHigh = 0x3d;
    static const uint8_t kAccelYOutLow = 0x3e;
    static const uint8_t kAccelZOutHigh = 0x3f;
    static const uint8_t kAccelZOutLow = 0x40;

    // register locations of temperature readings
    static const uint8_t kTempOutHigh = 0x41;
    static const uint8_t kTempOutLow = 0x42;

    // register locations of gyroscope x, y, z readings
    static const uint8_t kGyroXOutHigh = 0x43;
    static const uint8_t kGyroXOutLow = 0x44;
    static const uint8_t kGyroYOutHigh = 0x45;
    static const uint8_t kGyroYOutLow = 0x46;
    static const uint8_t kGyroZOutHigh = 0x47;
    static const uint8_t kGyroZOutLow = 0x48;

    // magnetometer register locations of magnetometer readings
    static const uint8_t kExtMagnoXOutHigh = 0x03;
    static const uint8_t kExtMagnoXOutLow = 0x04;
    static const uint8_t kExtMagnoYOutHigh = 0x05;
    static const uint8_t kExtMagnoYOutLow = 0x06;
    static const uint8_t kExtMagnoZOutHigh = 0x07;
    static const uint8_t kExtMagnoZOutLow = 0x08;

    // magnetometer register locations of the sensitivity adjustment values
    static const uint8_t kMagnoAdjustX = 0x10;
    static const uint8_t kMagnoAdjustY = 0x11;
    static const uint8_t kMagnoAdjustZ = 0x12;
};

#endif  // SRC_SENSORS_I2C_SENSORS_MPU9250_MOTION_TRACKER_H
