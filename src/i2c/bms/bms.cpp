#include <src/i2c/bms/bms.h>

Bms::Bms(I2c* bus, int address) :
        bus(bus), address(address)
{
    SetConfiguration();
}

void Bms::SetConfiguration()
{
    byte package[3];

    package[0] = Bms::kUVCLRegisterLocation;
    package[1] = Bms::kUVCLRegisterValue;
    package[2] = Bms::kEmptybuffervalue;
    bus->PerformWriteTransaction(address, package, 3);

    package[0] = Bms::kVChargeRegisterLocation;
    package[1] = Bms::kVChargeRegisterValue;
    package[2] = Bms::kEmptybuffervalue;
    bus->PerformWriteTransaction(address, package, 3);

    package[0] = Bms::kIChargeRegisterLocation;
    package[1] = Bms::kIChargeRegisterValue;
    package[2] = Bms::kEmptybuffervalue;
    bus->PerformWriteTransaction(address, package, 3);

    package[0] = Bms::kReChargeThresholdRegisterLocation;
    package[1] = Bms::kReChargeThresholdLRegisterValue;
    package[2] = Bms::kReChargeThresholdURegisterValue;
    bus->PerformWriteTransaction(address, package, 3);

    package[0] = Bms::kMaxCVTimeRegisterLocation;
    package[1] = Bms::kMaxCVTimeRegisterValue;
    package[2] = Bms::kEmptybuffervalue;
    bus->PerformWriteTransaction(address, package, 3);

    package[0] = Bms::kCXRegisterLocation;
    package[1] = Bms::kCXRegisterValue;
    package[2] = Bms::kEmptybuffervalue;
    bus->PerformWriteTransaction(address, package, 3);

    package[0] = Bms::kCXThresholdRegisterLocation;
    package[1] = Bms::kCXThresholdRegisterValue;
    package[2] = Bms::kEmptybuffervalue;
    bus->PerformWriteTransaction(address, package, 3);

    package[0] = Bms::kCoulomConfigRegisterLocation;
    package[1] = Bms::kCoulomConfigRegisterValue;
    package[2] = Bms::kEmptybuffervalue;
    bus->PerformWriteTransaction(address, package, 3);

    package[0] = Bms::kQCountInitialRegisterLocation;
    package[1] = Bms::kQCountInitialRegisterValue;
    package[2] = Bms::kEmptybuffervalue;
    bus->PerformWriteTransaction(address, package, 3);

    package[0] = Bms::kPrescaleFactorRegisterLocation;
    package[1] = Bms::kPrescaleFactorRegisterValue;
    package[2] = Bms::kEmptybuffervalue;
    bus->PerformWriteTransaction(address, package, 3);
}

//Read 2-byte from BMS Registers
uint16_t Bms::GetConfiguration(byte register_location,
                               etl::array<byte, 2>& read_buffer)
{
    SelectRegister(register_location);
    ReadFromCurrentRegister(read_buffer);
    uint16_t read_binary_reading = ((static_cast<uint16_t>(read_buffer.at(1)))
            << 8) | static_cast<uint16_t>(read_buffer.at(0));
    return read_binary_reading;
}

void Bms::SelectRegister(byte register_address)
{
    bus->PerformWriteTransaction(address, &register_address, 1);
}

void Bms::ReadFromCurrentRegister(etl::array<byte, 2>& read_buffer)
{
    byte i2c_buffer[2];
    if (bus->PerformReadTransaction(address, i2c_buffer, 2))
    {
        read_buffer.at(0) = i2c_buffer[0];
        read_buffer.at(1) = i2c_buffer[1];
    }
}

// Checking valid condition of BMS' Telemetry system
bool Bms::GetTelemetryValid(etl::array<byte, 2>& read_buffer)
{
    SelectRegister(kTelemetryValidRegisterLocation);
    ReadFromCurrentRegister(read_buffer);
    uint16_t Telemetry_system_binary_reading;
    Telemetry_system_binary_reading =
            ((static_cast<uint16_t>(read_buffer.at(1))) << 8)
                    | static_cast<uint16_t>(read_buffer.at(0));
    if (Telemetry_system_binary_reading & kTelemetryValidBitMask)
        return true;
    else
        return false;
}

// BMS Charge Status
Bms::ChargeStatus Bms::GetChargeStatus(etl::array<byte, 2>& read_buffer)
{
    if (GetTelemetryValid(read_buffer))
    {
        SelectRegister(kChargeStatusRegisterLocation);
        ReadFromCurrentRegister(read_buffer);
        uint16_t charge_status_binary_reading;
        charge_status_binary_reading =
                ((static_cast<uint16_t>(read_buffer.at(1))) << 8)
                        | static_cast<uint16_t>(read_buffer.at(0));
        if (charge_status_binary_reading & kConstantVoltageBitMask)
            return kConstantVoltage;
        else if (charge_status_binary_reading & kConstantCurrentBitMask)
            return kConstantCurrent;
        else if (charge_status_binary_reading & kIinLimitActiveBitMask)
            return kIinLimitActive;
        else if (charge_status_binary_reading & kVinLimitActiveBitMask)
            return kVinLimitActive;
        else if ((charge_status_binary_reading & kChargeStatusBitMask)
                == kChargeStatusNotCharging)
            return kNotCharging;
        else
            return kError;
    }
}

//BMS System Status
Bms::SystemStatus Bms::GetSystemStatus(etl::array<byte, 2>& read_buffer)
{
    if (GetTelemetryValid(read_buffer))
    {
        SelectRegister(kSystemStatusRegisterLocation);
        ReadFromCurrentRegister(read_buffer);
        uint16_t System_Status_binary_reading;
        System_Status_binary_reading =
                ((static_cast<uint16_t>(read_buffer.at(1))) << 8)
                        | static_cast<uint16_t>(read_buffer.at(0));
        if ((System_Status_binary_reading & kChargeEnableBitMask)
                == kChargeEnableBitMask)
            return kChargeEnable;
        else if ((System_Status_binary_reading & kChargeEnableBitMask)
                != kChargeEnableBitMask)
            return kChargeDisable;
        else
            return kOther;
    }
}
