#include <CppUTest/TestHarness.h>
#include <src/board/board.h>
#include <src/board/i2c/i2c.h>
#include <src/board/i2c/multiplexers/i2c_multiplexer.h>
#include <src/config/unit_tests.h>
#include <src/messages/MagnetometerReading.pb.h>
#include <src/sensors/i2c_sensors/mpu9250_motion_tracker.h>

static const uint8_t mpu9250_address = 104;  // assuming LSB is set to 0
static const std::string mpu9250_id = "mpu9250";
static const byte kMultiplexerAddress = 0x76;

static const double avg_room_temperature = 25.00;
static const double temp_tolerance = 15.00;

TEST_GROUP(MotionTracker) {
    void setup() {
        if (!imu_available) {
            TEST_EXIT;
        }
    };
};

TEST(MotionTracker, TestGyroRead) {
    I2c test_i2c_bus(I2C_BUS_A);
    I2cMultiplexer multiplexer(&test_i2c_bus, kMultiplexerAddress);
    multiplexer.OpenChannel(I2cMultiplexer::kMuxChannel1);

    MPU9250MotionTracker test_imu(&test_i2c_bus, mpu9250_address, mpu9250_id);

    GyroscopeReading gyroscope_reading;
    test_imu.TakeGyroscopeReading(gyroscope_reading);
    multiplexer.CloseAllChannels();

    CHECK(gyroscope_reading.x != 0.0);
    CHECK(gyroscope_reading.y != 0.0);
    CHECK(gyroscope_reading.z != 0.0);
}

TEST(MotionTracker, TestMagnoRead) {
    I2c test_i2c_bus(I2C_BUS_A);

    I2cMultiplexer multiplexer(&test_i2c_bus, kMultiplexerAddress);
    multiplexer.OpenChannel(I2cMultiplexer::kMuxChannel1);

    MPU9250MotionTracker test_imu(&test_i2c_bus, mpu9250_address, mpu9250_id);

    MagnetometerReading magnetometer_reading;
    test_imu.TakeMagnetometerReading(magnetometer_reading);
    multiplexer.CloseAllChannels();

    CHECK(magnetometer_reading.x != 0.0);
    CHECK(magnetometer_reading.y != 0.0);
    CHECK(magnetometer_reading.z != 0.0);
}

TEST(MotionTracker, TestTempRead) {
    I2c test_i2c_bus(I2C_BUS_A);

    I2cMultiplexer multiplexer(&test_i2c_bus, kMultiplexerAddress);
    multiplexer.OpenChannel(I2cMultiplexer::kMuxChannel1);

    MPU9250MotionTracker test_imu(&test_i2c_bus, mpu9250_address, mpu9250_id);

    Mpu9250TemperatureReading temperature_reading;
    test_imu.TakeTemperatureReading(temperature_reading);

    multiplexer.CloseAllChannels();
    DOUBLES_EQUAL(avg_room_temperature, temperature_reading.temp,
                  temp_tolerance);
}

TEST(MotionTracker, TestAccelRead) {
    I2c test_i2c_bus(I2C_BUS_A);

    I2cMultiplexer multiplexer(&test_i2c_bus, kMultiplexerAddress);
    multiplexer.OpenChannel(I2cMultiplexer::kMuxChannel1);

    MPU9250MotionTracker test_imu(&test_i2c_bus, mpu9250_address, mpu9250_id);

    AccelerometerReading accelerometer_reading;
    test_imu.TakeAccelerometerReading(accelerometer_reading);
    multiplexer.CloseAllChannels();

    CHECK(accelerometer_reading.x != 0.0);
    CHECK(accelerometer_reading.y != 0.0);
    CHECK(accelerometer_reading.z != 0.0);
}
