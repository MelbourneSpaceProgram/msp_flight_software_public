#ifndef SRC_SENSORS_I2C_SENSORS_I2C_DEVICE_H_
#define SRC_SENSORS_I2C_SENSORS_I2C_DEVICE_H_

#include <src/board/i2c/i2c.h>
#include <src/board/i2c/multiplexers/i2c_multiplexer.h>
#include <string>

class I2c;

class I2cDevice {
   public:
    I2cDevice(
        const I2c* bus, uint8_t address,
        const I2cMultiplexer* multiplexer = NULL,
        I2cMultiplexer::MuxChannel channel = I2cMultiplexer::kMuxNoChannel);

    uint8_t GetI2cAddress() const;
    const I2cMultiplexer* GetI2cMultiplexer() const;
    I2cMultiplexer::MuxChannel GetMultiplexerChannel() const;
    std::string GetInfoString() const;

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

   private:
    const I2c* bus;
    const I2cMultiplexer* multiplexer;
    I2cMultiplexer::MuxChannel channel;
    void MuxSelect() const;
    void MuxDeselect() const;
    std::string info_string;
    constexpr static uint8_t kInfoStringLength = 40;
    static GateMutexPri_Handle i2c_mutex;
};

#endif  //  SRC_SENSORS_I2C_SENSORS_I2C_DEVICE_H_
