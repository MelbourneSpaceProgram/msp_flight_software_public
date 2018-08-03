#ifndef SRC_SENSORS_I2C_SENSORS_I2C_DEVICE_H_
#define SRC_SENSORS_I2C_SENSORS_I2C_DEVICE_H_

#include <src/board/i2c/multiplexers/i2c_multiplexer.h>
#include <src/board/i2c/i2c.h>

class I2c;

class I2cDevice {
   public:
    I2cDevice(
        const I2c* bus, uint8_t address,
        const I2cMultiplexer* multiplexer = NULL,
        I2cMultiplexer::MuxChannel channel = I2cMultiplexer::kMuxNoChannel,
        bool failed = false);

    uint8_t GetI2cAddress() const;
    bool IsFailed() const;
    const I2cMultiplexer* GetI2cMultiplexer() const;
    I2cMultiplexer::MuxChannel GetMultiplexerChannel() const;

    bool PerformWriteTransaction(byte address, byte* write_buffer,
                                 uint16_t write_buffer_length) const;
    bool PerformReadTransaction(byte address, byte* write_buffer,
                                 uint16_t write_buffer_length) const;
    bool PerformTransaction(byte address, byte* read_buffer,
                                 uint16_t read_buffer_length, byte* write_buffer,
                                 uint16_t write_buffer_length) const;
    const I2c* GetI2cBus() const;

   protected:
    uint8_t address;
    void SetFailed(bool failed);

   private:
    const I2cMultiplexer* multiplexer;
    I2cMultiplexer::MuxChannel channel;
    bool failed;
    const I2c* bus;
    void MuxSelect() const;
    void MuxDeselect() const;
};

#endif  //  SRC_SENSORS_I2C_SENSORS_I2C_DEVICE_H_