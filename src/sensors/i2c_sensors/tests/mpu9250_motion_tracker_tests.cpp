#include <Board.h>
#include <src/config/unit_tests.h>
#include <src/i2c/i2c.h>
#include <src/i2c/multiplexers/i2c_multiplexer.h>
#include <src/messages/MagnetometerReading.pb.h>
#include <src/sensors/i2c_sensors/mpu9250_motion_tracker.h>
#include <test_runners/mpu9250_motion_tracker_tests.h>
#include <test_runners/unity.h>

static const uint8_t mpu9250_address = 104;  // assuming LSB is set to 0
static const std::string mpu9250_id = "mpu9250";
static const byte kMultiplexerAddress = 0x76;

static const double avg_room_temperature = 25.00;
static const double temp_tolerance = 15.00;

void TestGyroRead(void) {
    if (!imu_test_enabled) {
        TEST_IGNORE_MESSAGE("IMU test ignored");
    }
    I2c test_i2c_bus(I2C_BUS_A);
    I2cMultiplexer multiplexer(&test_i2c_bus, kMultiplexerAddress);
    multiplexer.OpenChannel(I2cMultiplexer::kMuxChannel1);

    MPU9250MotionTracker test_imu(&test_i2c_bus, mpu9250_address, mpu9250_id);

    GyroscopeReading gyroscope_reading;
    test_imu.TakeGyroscopeReading(gyroscope_reading);
    multiplexer.CloseAllChannels();

    TEST_ASSERT_TRUE(gyroscope_reading.x != 0.0);
    TEST_ASSERT_TRUE(gyroscope_reading.y != 0.0);
    TEST_ASSERT_TRUE(gyroscope_reading.z != 0.0);
}

void TestMagnoRead(void) {
    if (!imu_test_enabled) {
        TEST_IGNORE_MESSAGE("IMU test ignored");
    }
    I2c test_i2c_bus(I2C_BUS_A);

    I2cMultiplexer multiplexer(&test_i2c_bus, kMultiplexerAddress);
    multiplexer.OpenChannel(I2cMultiplexer::kMuxChannel1);

    MPU9250MotionTracker test_imu(&test_i2c_bus, mpu9250_address, mpu9250_id);

    MagnetometerReading magnetometer_reading;
    test_imu.TakeMagnetometerReading(magnetometer_reading);
    multiplexer.CloseAllChannels();

    TEST_ASSERT_TRUE(magnetometer_reading.x != 0.0);
    TEST_ASSERT_TRUE(magnetometer_reading.y != 0.0);
    TEST_ASSERT_TRUE(magnetometer_reading.z != 0.0);
}

void TestTempRead(void) {
    if (!imu_test_enabled) {
        TEST_IGNORE_MESSAGE("IMU test ignored");
    }
    I2c test_i2c_bus(I2C_BUS_A);

    I2cMultiplexer multiplexer(&test_i2c_bus, kMultiplexerAddress);
    multiplexer.OpenChannel(I2cMultiplexer::kMuxChannel1);

    MPU9250MotionTracker test_imu(&test_i2c_bus, mpu9250_address, mpu9250_id);

    TemperatureReading temperature_reading;
    test_imu.TakeTemperatureReading(temperature_reading);

    multiplexer.CloseAllChannels();
    TEST_ASSERT_DOUBLE_WITHIN(temp_tolerance, avg_room_temperature,
                              temperature_reading.temp);
}

void TestAccelRead(void) {
    if (!imu_test_enabled) {
        TEST_IGNORE_MESSAGE("IMU test ignored");
    }
    I2c test_i2c_bus(I2C_BUS_A);

    I2cMultiplexer multiplexer(&test_i2c_bus, kMultiplexerAddress);
    multiplexer.OpenChannel(I2cMultiplexer::kMuxChannel1);

    MPU9250MotionTracker test_imu(&test_i2c_bus, mpu9250_address, mpu9250_id);

    AccelerometerReading accelerometer_reading;
    test_imu.TakeAccelerometerReading(accelerometer_reading);
    multiplexer.CloseAllChannels();

    TEST_ASSERT_TRUE(accelerometer_reading.x != 0.0);
    TEST_ASSERT_TRUE(accelerometer_reading.y != 0.0);
    TEST_ASSERT_TRUE(accelerometer_reading.z != 0.0);
}
