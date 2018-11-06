#ifndef SRC_BOARD_I2C_IO_EXPANDER_IO_EXPANDER_H_
#define SRC_BOARD_I2C_IO_EXPANDER_IO_EXPANDER_H_

#include <src/board/i2c/multiplexers/i2c_multiplexer.h>
#include <src/sensors/i2c_sensors/i2c_device.h>

class I2c;

//
// Controls TCA9535 Low-Voltage 16-Bit I2C and SMBus
// Low-Power I/O Expander with Interrupt Output
// and Configuration Registers.
//
// Datasheet:
// http://www.ti.com/lit/ds/symlink/tca9535.pdf
//

class IoExpander : public I2cDevice {
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
        kIoPin10 = 0x08,
        kIoPin11 = 0x09,
        kIoPin12 = 0x0A,
        kIoPin13 = 0x0B,
        kIoPin14 = 0x0C,
        kIoPin15 = 0x0D,
		kNumIoExpanderPins
    };

    enum IoExpanderIds {
        kEpsIoExpander = 0,
        kCommsIoExpander = 1,
        kNumIoExpanders
    };

    enum IoDirection { kIoInput, kIoOutput };

    enum IoPolarity { kIoActiveHigh, kIoActiveLow };

    static void Init(I2c* bus_d);
    static const IoExpander* GetIoExpander(uint8_t index);

    // TODO(dingbenjamin): Make constructor private after friending unit test
    IoExpander(
        const I2c* bus, byte address, const I2cMultiplexer* multiplexer = NULL,
        I2cMultiplexer::MuxChannel channel = I2cMultiplexer::kMuxNoChannel);

    IoDirection GetDirection(IoPin pin) const;
    void SetDirection(IoPin pin, IoDirection direction) const;
    bool GetPin(IoPin pin) const;
    void SetPin(IoPin pin, bool value) const;
    IoPolarity GetPolarity(IoPin pin) const;
    void SetPolarity(IoPin pin, IoPolarity polarity) const;

    // Useful pin definitions
    static constexpr byte kBmsIoExpanderAddress = 0x20;
    static constexpr byte kTelecomsIoExpanderAddress = 0x26;
    static constexpr IoPin kIoExpanderPinBms1En = kIoPin0;
    static constexpr IoPin kIoExpanderPinBms2En = kIoPin2;
    static constexpr IoPin kIoExpanderPinFSEn = kIoPin15;
    static constexpr IoPin kIoExpanderPinTelecomsEn1 = kIoPin1;
    static constexpr IoPin kIoExpanderPinTelecomsEn3 = kIoPin1;
    static constexpr IoPin kIoExpanderPinTelecomsEn4 = kIoPin4;
    static constexpr IoPin kIoExpanderPinTelecomsEn5 = kIoPin4;

   private:
    static const IoExpander* io_expanders[kNumIoExpanders];
    static bool initialised;

    uint8_t ReadFromRegister(byte register_address) const;
    void WriteToRegister(byte register_address, byte value) const;
    uint8_t GetRegisterMask(IoPin pin) const;
    bool GetBitForPin(IoPin pin, uint8_t low_register,
                      uint8_t high_register) const;
    bool SetBitForPin(IoPin pin, uint8_t low_register, uint8_t high_register,
                      bool value) const;

    //
    // Datasheet ref
    // Table 3. Command Byte
    //
    static constexpr uint8_t kInputPort0 = 0x00;
    static constexpr uint8_t kInputPort1 = 0x01;
    static constexpr uint8_t kOutputPort0 = 0x02;
    static constexpr uint8_t kOutputPort1 = 0x03;
    static constexpr uint8_t kPolarityInversionPort0 = 0x04;
    static constexpr uint8_t kPolarityInversionPort1 = 0x05;
    static constexpr uint8_t kConfigurationPort0 = 0x06;
    static constexpr uint8_t kConfigurationPort1 = 0x07;
};

#endif  // SRC_BOARD_I2C_IO_EXPANDER_IO_EXPANDER_H_
