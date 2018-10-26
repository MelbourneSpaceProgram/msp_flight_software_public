#include <external/etl/exception.h>
#include <src/sensors/i2c_sensors/mlx90615.h>

Mlx90615::Mlx90615(const I2c* bus, int address,
                   const I2cMultiplexer* multiplexer,
                   I2cMultiplexer::MuxChannel channel)
    : I2cDevice(bus, address, multiplexer, channel) {}

float Mlx90615::ReadInfrared() { return ReadRegister(kInfraredRegister); }

float Mlx90615::ReadDeviceTemperature() {
    return ReadRegister(kDeviceTemperatureRegister);
}

float Mlx90615::ReadRegister(uint8_t Mlx90615Register) {
    uint8_t infrared_reading[3];
    uint8_t write_buffer[1];
    write_buffer[0] = Mlx90615Register;
    bool reading_successful =
        PerformTransaction(address, infrared_reading, 3, write_buffer, 1);
    float infrared = ((static_cast<int16_t>(infrared_reading[1])) << 8) |
                     static_cast<int16_t>(infrared_reading[2]);
    infrared = kIrScaleFactor * infrared - kIrOffset;

    if (reading_successful) {
        return infrared;
    } else {
        etl::exception e("Failed MLX90615 Reading", __FILE__, __LINE__);
        throw e;
    }
}
