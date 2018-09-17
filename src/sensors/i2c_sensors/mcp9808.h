#ifndef SRC_SENSORS_I2C_SENSORS_MCP9808_H_
#define SRC_SENSORS_I2C_SENSORS_MCP9808_H_

#include <src/sensors/i2c_sensors/i2c_device.h>

class Mcp9808 : public I2cDevice {
   public:
    Mcp9808(const I2c* bus, uint8_t address,
            const I2cMultiplexer* multiplexer = NULL,
            I2cMultiplexer::MuxChannel channel = I2cMultiplexer::kMuxNoChannel);

    double TakeI2cReading();

   private:
    static constexpr uint8_t kTempRegister = 0x05;
    static constexpr byte kSignBitMask = 0b00010000;
    static constexpr byte kUpperByteMask = 0b00011111;
    static constexpr double kUpperByteScale = 16;
    static constexpr double kLowerByteScale = 1.0 / 16;
};

#endif  //  SRC_SENSORS_I2C_SENSORS_MCP9808_H_
