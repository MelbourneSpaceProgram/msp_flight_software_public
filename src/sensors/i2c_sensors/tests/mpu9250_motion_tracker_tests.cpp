#include <CppUTest/TestHarness.h>
#include <src/board/board.h>
#include <src/board/i2c/i2c.h>
#include <src/board/i2c/multiplexers/i2c_multiplexer.h>
#include <src/config/unit_tests.h>
#include <src/messages/MagnetometerReading.pb.h>
#include <src/sensors/i2c_sensors/mpu9250_motion_tracker.h>
#include <src/util/msp_exception.h>
#include <xdc/runtime/Log.h>

static constexpr uint8_t mpu9250_address = 0x68;
static constexpr byte kMultiplexerAddress = 0x76;

static constexpr double avg_room_temperature = 25.00;
static constexpr double temp_tolerance = 15.00;

TEST_GROUP(MotionTracker) {
    void setup() {
        if (!SystemConfiguration::GetInstance()->IsI2cAvailable() || !SystemConfiguration::GetInstance()->IsFsBoardAvailable()) {
            TEST_EXIT;
        }
    };
};

TEST(MotionTracker, TestGyroRead) {
    try {
        I2c test_i2c_bus(I2C_BUS_A);
        I2cMultiplexer multiplexer(&test_i2c_bus, kMultiplexerAddress);

        MPU9250MotionTracker test_imu(&test_i2c_bus, mpu9250_address,
                                      &multiplexer,
                                      I2cMultiplexer::kMuxChannel1);

        GyroscopeReading gyroscope_reading;
        gyroscope_reading = test_imu.TakeGyroscopeReading();

        int16_t full_scale = 250;  // Default full scale range
        CHECK_COMPARE(gyroscope_reading.x, <, full_scale);
        CHECK_COMPARE(gyroscope_reading.y, <, full_scale);
        CHECK_COMPARE(gyroscope_reading.z, <, full_scale);

        CHECK_COMPARE(gyroscope_reading.x, >, -full_scale);
        CHECK_COMPARE(gyroscope_reading.y, >, -full_scale);
        CHECK_COMPARE(gyroscope_reading.z, >, -full_scale);

        if (SystemConfiguration::GetInstance()->IsVerboseLogging())
            Log_info3("Gyro: x: %f | y: %f | z: %f", gyroscope_reading.x,
                      gyroscope_reading.y, gyroscope_reading.z);
    } catch (MspException& e) {
        FAIL("Uncaught exception in test");
    }
}

TEST(MotionTracker, TestMagnoRead) {
    try {
        I2c test_i2c_bus(I2C_BUS_A);

        I2cMultiplexer multiplexer(&test_i2c_bus, kMultiplexerAddress);

        MPU9250MotionTracker test_imu(&test_i2c_bus, mpu9250_address,
                                      &multiplexer,
                                      I2cMultiplexer::kMuxChannel1);

        MagnetometerReading magnetometer_reading;

        magnetometer_reading = test_imu.TakeMagnetometerReading();

        int16_t full_scale = 4800;  // Datasheet
        CHECK_COMPARE(magnetometer_reading.x, <, full_scale);
        CHECK_COMPARE(magnetometer_reading.y, <, full_scale);
        CHECK_COMPARE(magnetometer_reading.z, <, full_scale);

        CHECK_COMPARE(magnetometer_reading.x, >, -full_scale);
        CHECK_COMPARE(magnetometer_reading.y, >, -full_scale);
        CHECK_COMPARE(magnetometer_reading.z, >, -full_scale);

        if (SystemConfiguration::GetInstance()->IsVerboseLogging())
            Log_info3("Magno: x: %f | y: %f | z: %f", magnetometer_reading.x,
                      magnetometer_reading.y, magnetometer_reading.z);
    } catch (MspException& e) {
        FAIL("Uncaught exception in test");
    }
}

TEST(MotionTracker, TestTempRead) {
    try {
        I2c test_i2c_bus(I2C_BUS_A);

        I2cMultiplexer multiplexer(&test_i2c_bus, kMultiplexerAddress);

        MPU9250MotionTracker test_imu(&test_i2c_bus, mpu9250_address,
                                      &multiplexer,
                                      I2cMultiplexer::kMuxChannel1);

        double temperature_reading;

        temperature_reading = test_imu.TakeTemperatureReading();
        DOUBLES_EQUAL(avg_room_temperature, temperature_reading,
                      temp_tolerance);
    } catch (MspException& e) {
        FAIL("Uncaught exception in test");
    }
}

TEST(MotionTracker, TestAccelRead) {
    try {
        I2c test_i2c_bus(I2C_BUS_A);

        I2cMultiplexer multiplexer(&test_i2c_bus, kMultiplexerAddress);

        MPU9250MotionTracker test_imu(&test_i2c_bus, mpu9250_address,
                                      &multiplexer,
                                      I2cMultiplexer::kMuxChannel1);

        AccelerometerReading accelerometer_reading;

        accelerometer_reading = test_imu.TakeAccelerometerReading();

        int16_t full_scale = 2;  // Default full scale range
        CHECK_COMPARE(accelerometer_reading.x, <, full_scale);
        CHECK_COMPARE(accelerometer_reading.y, <, full_scale);
        CHECK_COMPARE(accelerometer_reading.z, <, full_scale);

        CHECK_COMPARE(accelerometer_reading.x, >, -full_scale);
        CHECK_COMPARE(accelerometer_reading.y, >, -full_scale);
        CHECK_COMPARE(accelerometer_reading.z, >, -full_scale);

        if (SystemConfiguration::GetInstance()->IsVerboseLogging())
            Log_info3("Magno: x: %f | y: %f | z: %f", accelerometer_reading.x,
                      accelerometer_reading.y, accelerometer_reading.z);
    } catch (MspException& e) {
        FAIL("Uncaught exception in test");
    }
}
