#include <assert.h>
#include <src/board/i2c/i2c.h>
#include <src/board/i2c/io_expander/io_expander.h>
#include <src/util/etl_utils.h>
#include <xdc/runtime/Log.h>

const IoExpander* IoExpander::io_expanders[IoExpander::kNumIoExpanders];
bool IoExpander::initialised = false;

void IoExpander::Init(I2c* bus_d) {
    io_expanders[kEpsIoExpander] = new IoExpander(bus_d, kBmsIoExpanderAddress);
    io_expanders[kCommsIoExpander] =
        new IoExpander(bus_d, kTelecomsIoExpanderAddress);

    const IoExpander* io_expander_bms = io_expanders[kEpsIoExpander];
    const IoExpander* io_expander_telecoms = io_expanders[kCommsIoExpander];

    try {
        io_expander_bms->SetDirection(kIoExpanderPinBms1En, kIoOutput);
        io_expander_bms->SetDirection(kIoExpanderPinBms2En, kIoOutput);
        io_expander_bms->SetPolarity(kIoExpanderPinBms1En, kIoActiveHigh);
        io_expander_bms->SetPolarity(kIoExpanderPinBms2En, kIoActiveHigh);

        io_expander_bms->SetDirection(kIoExpanderPinFSEn, kIoOutput);
        io_expander_bms->SetPolarity(kIoExpanderPinFSEn, kIoActiveHigh);

        io_expander_bms->SetPin(kIoExpanderPinBms1En, true);
        io_expander_bms->SetPin(kIoExpanderPinBms2En, true);
        io_expander_bms->SetPin(kIoExpanderPinFSEn, true);
    } catch (etl::exception& e) {
        EtlUtils::LogException(e);
        Log_error0("BMS IO expander failed to initialise properly");
    }

    try {
        io_expander_telecoms->SetDirection(kIoExpanderPinTelecomsEn1,
                                           kIoOutput);
        io_expander_telecoms->SetDirection(kIoExpanderPinTelecomsEn3,
                                           kIoOutput);
        io_expander_telecoms->SetDirection(kIoExpanderPinTelecomsEn4,
                                           kIoOutput);
        io_expander_telecoms->SetDirection(kIoExpanderPinTelecomsEn5,
                                           kIoOutput);
        io_expander_telecoms->SetPolarity(kIoExpanderPinTelecomsEn1,
                                          kIoActiveHigh);
        io_expander_telecoms->SetPolarity(kIoExpanderPinTelecomsEn3,
                                          kIoActiveHigh);
        io_expander_telecoms->SetPolarity(kIoExpanderPinTelecomsEn4,
                                          kIoActiveHigh);
        io_expander_telecoms->SetPolarity(kIoExpanderPinTelecomsEn5,
                                          kIoActiveHigh);

        io_expander_telecoms->SetPin(kIoExpanderPinTelecomsEn1, true);
        io_expander_telecoms->SetPin(kIoExpanderPinTelecomsEn3, true);
        io_expander_telecoms->SetPin(kIoExpanderPinTelecomsEn4, true);
        io_expander_telecoms->SetPin(kIoExpanderPinTelecomsEn5, true);
    } catch (etl::exception& e) {
        EtlUtils::LogException(e);
        Log_error0("Telecomms IO expander failed to initialise properly");
    }

    initialised = true;
}

const IoExpander* IoExpander::GetIoExpander(uint8_t index) {
    assert(initialised);
    if (!initialised) {
        throw etl::exception("IO expanders uninitialised", __FILE__, __LINE__);
    } else if (index >= kNumIoExpanders) {
        // TODO(dingbenjamin): Include index in exceptions
        throw etl::exception("IO expander index out of range", __FILE__,
                             __LINE__);
    } else if (io_expanders[index] == NULL) {
        throw etl::exception("IO expander index contains NULL", __FILE__,
                             __LINE__);
    }
    return io_expanders[index];
}

IoExpander::IoExpander(const I2c* bus, byte address,
                       const I2cMultiplexer* multiplexer,
                       I2cMultiplexer::MuxChannel channel)
    : I2cDevice(bus, address, multiplexer, channel) {}

IoExpander::IoDirection IoExpander::GetDirection(IoExpander::IoPin pin) const {
    // 1 = Input
    // 0 = Output

    bool value = GetBitForPin(pin, kConfigurationPort0, kConfigurationPort1);

    if (value) {
        return kIoInput;
    } else {
        return kIoOutput;
    }
}

void IoExpander::SetDirection(IoExpander::IoPin pin,
                              IoExpander::IoDirection direction) const {
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
}

bool IoExpander::GetPin(IoExpander::IoPin pin) const {
    // 1 = High
    // 0 = Low

    bool value = GetBitForPin(pin, kInputPort0, kInputPort1);

    return value;
}

void IoExpander::SetPin(IoExpander::IoPin pin, bool value) const {
    // 1 = High
    // 0 = Low

    SetBitForPin(pin, kOutputPort0, kOutputPort1, value);
}

IoExpander::IoPolarity IoExpander::GetPolarity(IoExpander::IoPin pin) const {
    // 1 = Active Low
    // 0 = Active High

    bool value =
        GetBitForPin(pin, kPolarityInversionPort0, kPolarityInversionPort1);

    if (value) {
        return kIoActiveLow;
    } else {
        return kIoActiveHigh;
    }
}

void IoExpander::SetPolarity(IoExpander::IoPin pin,
                             IoExpander::IoPolarity polarity) const {
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
}

uint8_t IoExpander::ReadFromRegister(byte register_address) const {
    uint8_t current_value = 0x00;
    if (!PerformWriteTransaction(address, &register_address, 1))
        throw etl::exception("Failed to write to address on IO expander",
                             __FILE__, __LINE__);
    if (!PerformReadTransaction(address, &current_value, 1))
        throw etl::exception("Failed to read from address on IO expander",
                             __FILE__, __LINE__);
    return current_value;
}

void IoExpander::WriteToRegister(byte register_address, byte value) const {
    byte bytes[2];
    bytes[0] = register_address;
    bytes[1] = value;
    if (!PerformWriteTransaction(address, bytes, 2)) {
        throw etl::exception("Failed to write to address on IO expander",
                             __FILE__, __LINE__);
    }
}

uint8_t IoExpander::GetRegisterMask(IoExpander::IoPin pin) const {
    return 0x01 << static_cast<uint8_t>(pin % 8);
}

bool IoExpander::GetBitForPin(IoExpander::IoPin pin, uint8_t low_register,
                              uint8_t high_register) const {
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

bool IoExpander::SetBitForPin(IoExpander::IoPin pin, uint8_t low_register,
                              uint8_t high_register, bool value) const {
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
