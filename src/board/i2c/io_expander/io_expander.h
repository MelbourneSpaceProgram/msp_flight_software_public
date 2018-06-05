#ifndef SRC_BOARD_I2C_IO_EXPANDER_IO_EXPANDER_H_
#define SRC_BOARD_I2C_IO_EXPANDER_IO_EXPANDER_H_

#include <src/board/i2c/multiplexers/i2c_multiplexer.h>

class I2c;

//
// Controls TCA9535 Low-Voltage 16-Bit I2C and SMBus
// Low-Power I/O Expander with Interrupt Output
// and Configuration Registers.
//
// Datasheet:
// http://www.ti.com/lit/ds/symlink/tca9535.pdf
//

class I2cIoExpander {
   public:
    enum IoPin {
        kIoPin0 = 0x00,
        kIoPin1 = 0x01,
        kIoPin2 = 0x02,
        kIoPin3 = 0x03,
        kIoPin4 = 0x04,
        kIoPin5 = 0x05,
        kIoPin6 = 0x06,
        kIoPin7 = 0x07,
        kIoPin8 = 0x08,
        kIoPin9 = 0x09,
        kIoPin10 = 0x0A,
        kIoPin11 = 0x0B,
        kIoPin12 = 0x0C,
        kIoPin13 = 0x0D,
        kIoPin14 = 0x0E,
        kIoPin15 = 0x0F
    };

    enum IoDirection { kIoInput, kIoOutput };

    enum IoPolarity { kIoActiveHigh, kIoActiveLow };

    I2cIoExpander(
        const I2c* bus, byte address, const I2cMultiplexer* multiplexer = NULL,
        I2cMultiplexer::MuxChannel channel = I2cMultiplexer::kMuxNoChannel);

    IoDirection GetDirection(IoPin pin);
    void SetDirection(IoPin pin, IoDirection direction);
    bool GetPin(IoPin pin);
    void SetPin(IoPin pin, bool value);
    IoPolarity GetPolarity(IoPin pin);
    void SetPolarity(IoPin pin, IoPolarity polarity);

   private:
    const I2c* bus;
    const byte address;
    const I2cMultiplexer* multiplexer;
    const I2cMultiplexer::MuxChannel channel;
    void SelectMultiplexerLine();
    void DeselectMultiplexerLine();
    uint8_t ReadFromRegister(byte register_address);
    void WriteToRegister(byte register_address, byte value);
    uint8_t GetRegisterMask(IoPin pin);
    bool GetBitForPin(IoPin pin, uint8_t low_register, uint8_t high_register);
    bool SetBitForPin(IoPin pin, uint8_t low_register, uint8_t high_register,
                      bool value);

    //
    // Datasheet ref
    // Table 3. Command Byte
    //
    static const uint8_t kInputPort0 = 0x00;
    static const uint8_t kInputPort1 = 0x01;
    static const uint8_t kOutputPort0 = 0x02;
    static const uint8_t kOutputPort1 = 0x03;
    static const uint8_t kPolarityInversionPort0 = 0x04;
    static const uint8_t kPolarityInversionPort1 = 0x05;
    static const uint8_t kConfigurationPort0 = 0x06;
    static const uint8_t kConfigurationPort1 = 0x07;
};

#endif  // SRC_BOARD_I2C_IO_EXPANDER_IO_EXPANDER_H_
