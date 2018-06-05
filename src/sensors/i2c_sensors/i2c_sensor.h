#ifndef SRC_SENSORS_I2C_SENSORS_I2C_SENSOR_H_
#define SRC_SENSORS_I2C_SENSORS_I2C_SENSOR_H_


#include <src/board/i2c/multiplexers/i2c_multiplexer.h>

class I2c;

class I2cSensor {
   public:
    I2cSensor(
        const I2c* bus, uint8_t address,
        const I2cMultiplexer* multiplexer = NULL,
        I2cMultiplexer::MuxChannel channel = I2cMultiplexer::kMuxNoChannel);

    void MuxSelect() const;
    void MuxDeselect() const;
    const I2c* GetI2cBus() const;
    uint8_t GetI2cAddress() const;
    const I2cMultiplexer* GetI2cMultiplexer() const;
    I2cMultiplexer::MuxChannel GetMultiplexerChannel() const;

   protected:
    const I2c* bus;
    uint8_t address;

   private:
    const I2cMultiplexer* multiplexer;
    I2cMultiplexer::MuxChannel channel;
};

#endif  //  SRC_SENSORS_I2C_SENSORS_I2C_SENSOR_H_
