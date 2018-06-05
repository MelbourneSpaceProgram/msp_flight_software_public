#include <src/board/i2c/i2c.h>
#include <src/board/i2c/io_expander/io_expander.h>
#include <xdc/runtime/Log.h>

I2cIoExpander::I2cIoExpander(const I2c* bus, byte address,
                             const I2cMultiplexer* multiplexer,
                             I2cMultiplexer::MuxChannel channel)
    : bus(bus), address(address), multiplexer(multiplexer), channel(channel) {}

I2cIoExpander::IoDirection I2cIoExpander::GetDirection(
    I2cIoExpander::IoPin pin) {
    SelectMultiplexerLine();

    // 1 = Input
    // 0 = Output

    bool value = GetBitForPin(pin, kConfigurationPort0, kConfigurationPort1);

    DeselectMultiplexerLine();

    if (value) {
        return kIoInput;
    } else {
        return kIoOutput;
    }
}

void I2cIoExpander::SetDirection(I2cIoExpander::IoPin pin,
                                 I2cIoExpander::IoDirection direction) {
    SelectMultiplexerLine();

    // 1 = Input
    // 0 = Output

    bool value;
    if (direction == kIoInput) {
        value = true;
    } else if (direction == kIoOutput) {
        value = false;
    } else {
        throw etl::exception("Invalid direction", __FILE__, __LINE__);
    }

    SetBitForPin(pin, kConfigurationPort0, kConfigurationPort1, value);

    DeselectMultiplexerLine();
}

bool I2cIoExpander::GetPin(I2cIoExpander::IoPin pin) {
    SelectMultiplexerLine();

    // 1 = High
    // 0 = Low

    bool value = GetBitForPin(pin, kInputPort0, kInputPort1);

    DeselectMultiplexerLine();

    return value;
}

void I2cIoExpander::SetPin(I2cIoExpander::IoPin pin, bool value) {
    SelectMultiplexerLine();

    // 1 = High
    // 0 = Low

    SetBitForPin(pin, kOutputPort0, kOutputPort1, value);

    DeselectMultiplexerLine();
}

I2cIoExpander::IoPolarity I2cIoExpander::GetPolarity(I2cIoExpander::IoPin pin) {
    SelectMultiplexerLine();

    // 1 = Active Low
    // 0 = Active High

    bool value =
        GetBitForPin(pin, kPolarityInversionPort0, kPolarityInversionPort1);

    DeselectMultiplexerLine();

    if (value) {
        return kIoActiveLow;
    } else {
        return kIoActiveHigh;
    }
}

void I2cIoExpander::SetPolarity(I2cIoExpander::IoPin pin,
                                I2cIoExpander::IoPolarity polarity) {
    SelectMultiplexerLine();

    // 1 = Active Low
    // 0 = Active High

    bool value;
    if (polarity == kIoActiveLow) {
        value = true;
    } else if (polarity == kIoActiveHigh) {
        value = false;
    } else {
        throw etl::exception("Invalid polarity", __FILE__, __LINE__);
    }

    SetBitForPin(pin, kPolarityInversionPort0, kPolarityInversionPort1, value);

    DeselectMultiplexerLine();
}

void I2cIoExpander::SelectMultiplexerLine() {
    if (multiplexer != NULL) {
        multiplexer->OpenChannel(channel);
    }
}

void I2cIoExpander::DeselectMultiplexerLine() {
    if (multiplexer != NULL) {
        multiplexer->CloseChannel(channel);
    }
}

uint8_t I2cIoExpander::ReadFromRegister(byte register_address) {
    uint8_t current_value = 0x00;
    bus->PerformWriteTransaction(address, &register_address, 1);
    bus->PerformReadTransaction(address, &current_value, 1);
    return current_value;
}

void I2cIoExpander::WriteToRegister(byte register_address, byte value) {
    byte bytes[2];
    bytes[0] = register_address;
    bytes[1] = value;
    bus->PerformWriteTransaction(address, bytes, 2);
}

uint8_t I2cIoExpander::GetRegisterMask(I2cIoExpander::IoPin pin) {
    return 0x01 << static_cast<uint8_t>(pin % 8);
}

bool I2cIoExpander::GetBitForPin(I2cIoExpander::IoPin pin, uint8_t low_register,
                                 uint8_t high_register) {
    // The lower register handles the lower 8 pins.
    // The higher register handles the upper 8 pins.
    //
    // Switch to the correct register for this pin.
    uint8_t i2c_register;
    if (static_cast<uint8_t>(pin) < 8) {
        i2c_register = low_register;
    } else {
        i2c_register = high_register;
    }

    // Read the current register state
    uint8_t current_value = ReadFromRegister(i2c_register);

    // Check if the bit representing the current pin is set
    uint8_t mask = GetRegisterMask(pin);

    if (current_value & mask) {
        return true;
    } else {
        return false;
    }
}

bool I2cIoExpander::SetBitForPin(I2cIoExpander::IoPin pin, uint8_t low_register,
                                 uint8_t high_register, bool value) {
    // The lower register handles the lower 8 pins.
    // The higher register handles the upper 8 pins.
    //
    // Switch to the correct register for this pin.
    uint8_t i2c_register;
    if (static_cast<uint8_t>(pin) < 8) {
        i2c_register = low_register;
    } else {
        i2c_register = high_register;
    }

    // Read the current register state
    uint8_t current_value = ReadFromRegister(i2c_register);

    // Apply the pin change over the top of current register state
    // by setting the bit that represents this pin.
    if (value) {
        current_value |= GetRegisterMask(pin);
    } else {
        current_value &= ~GetRegisterMask(pin);
    }

    // Write the new register state
    WriteToRegister(i2c_register, current_value);
    return true;
}
