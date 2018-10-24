#include <CppUTest/TestHarness.h>
#include <src/board/board.h>
#include <src/board/i2c/i2c.h>
#include <src/board/i2c/multiplexers/i2c_multiplexer.h>
#include <src/config/unit_tests.h>
#include <src/messages/MagnetometerReading.pb.h>
#include <src/sensors/i2c_sensors/mpu9250_motion_tracker.h>

static constexpr uint8_t mpu9250_address = 0x68;
static constexpr byte kMultiplexerAddress = 0x76;

static constexpr double avg_room_temperature = 25.00;
static constexpr double temp_tolerance = 15.00;

TEST_GROUP(MotionTracker) {
    void setup() {
        if (!kI2cAvailable || !kFsBoardAvailable) {
            TEST_EXIT;
        }
    };
};

TEST(MotionTracker, TestGyroRead) {
    I2c test_i2c_bus(I2C_BUS_A);
    I2cMultiplexer multiplexer(&test_i2c_bus, kMultiplexerAddress);

    MPU9250MotionTracker test_imu(&test_i2c_bus, mpu9250_address, &multiplexer,
                                  I2cMultiplexer::kMuxChannel1);

    GyroscopeReading gyroscope_reading;

    multiplexer.OpenChannel(I2cMultiplexer::kMuxChannel1);
    gyroscope_reading = test_imu.TakeGyroscopeReading();
    multiplexer.CloseAllChannels();

    CHECK(gyroscope_reading.x != 0.0);
    CHECK(gyroscope_reading.y != 0.0);
    CHECK(gyroscope_reading.z != 0.0);
}

TEST(MotionTracker, TestMagnoRead) {
    I2c test_i2c_bus(I2C_BUS_A);

    I2cMultiplexer multiplexer(&test_i2c_bus, kMultiplexerAddress);

    MPU9250MotionTracker test_imu(&test_i2c_bus, mpu9250_address, &multiplexer,
                                  I2cMultiplexer::kMuxChannel1);

    MagnetometerReading magnetometer_reading;

    multiplexer.OpenChannel(I2cMultiplexer::kMuxChannel1);
    magnetometer_reading = test_imu.TakeMagnetometerReading();
    multiplexer.CloseAllChannels();

    CHECK(magnetometer_reading.x != 0.0);
    CHECK(magnetometer_reading.y != 0.0);
    CHECK(magnetometer_reading.z != 0.0);
}

TEST(MotionTracker, TestTempRead) {
    I2c test_i2c_bus(I2C_BUS_A);

    I2cMultiplexer multiplexer(&test_i2c_bus, kMultiplexerAddress);

    MPU9250MotionTracker test_imu(&test_i2c_bus, mpu9250_address, &multiplexer,
                                  I2cMultiplexer::kMuxChannel1);

    double temperature_reading;

    multiplexer.OpenChannel(I2cMultiplexer::kMuxChannel1);
    temperature_reading = test_imu.TakeTemperatureReading();
    multiplexer.CloseAllChannels();

    DOUBLES_EQUAL(avg_room_temperature, temperature_reading, temp_tolerance);
}

TEST(MotionTracker, TestAccelRead) {
    I2c test_i2c_bus(I2C_BUS_A);

    I2cMultiplexer multiplexer(&test_i2c_bus, kMultiplexerAddress);

    MPU9250MotionTracker test_imu(&test_i2c_bus, mpu9250_address, &multiplexer,
                                  I2cMultiplexer::kMuxChannel1);

    AccelerometerReading accelerometer_reading;

    multiplexer.OpenChannel(I2cMultiplexer::kMuxChannel1);
    accelerometer_reading = test_imu.TakeAccelerometerReading();
    multiplexer.CloseAllChannels();

    CHECK(accelerometer_reading.x != 0.0);
    CHECK(accelerometer_reading.y != 0.0);
    CHECK(accelerometer_reading.z != 0.0);
}
