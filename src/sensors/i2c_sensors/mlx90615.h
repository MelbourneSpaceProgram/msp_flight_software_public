#ifndef I2C_SENSORS_MLX90615_H_
#define I2C_SENSORS_MLX90615_H_

#include <src/sensors/i2c_sensors/i2c_device.h>

class Mlx90615 : I2cDevice {
   public:
    Mlx90615(
        const I2c* bus, int address, const I2cMultiplexer* multiplexer = NULL,
        I2cMultiplexer::MuxChannel channel = I2cMultiplexer::kMuxNoChannel);

    float ReadDeviceTemperature();
    float ReadInfrared();
    float ReadRegister(uint8_t);

   private:
    static constexpr int kDeviceTemperatureRegister = 0x26;
    static constexpr int kInfraredRegister = 0x27;
    static constexpr float kIrScaleFactor = 0.02;
    static constexpr float kIrOffset = 273.15;
};

#endif /* I2C_SENSORS_MLX90615_H_ */
