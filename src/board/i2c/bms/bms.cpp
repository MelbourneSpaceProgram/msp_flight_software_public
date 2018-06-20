#include <math.h>
#include <src/board/i2c/bms/bms.h>
#include <src/board/i2c/i2c.h>

Bms::Bms(const I2c* bus, int address, const I2cMultiplexer* multiplexer,
         I2cMultiplexer::MuxChannel channel) :
        I2cSensor(bus, address, multiplexer, channel)
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

    package[0] = Bms::kCXJeitaEnableRegisterLocation;
    package[1] = Bms::kCXJeitaEnableRegisterValue;
    package[2] = Bms::kEmptybuffervalue;
    bus->PerformWriteTransaction(address, package, 3);

    package[0] = Bms::kCXThresholdRegisterLocation;
    package[1] = Bms::kCXThresholdRegisterValue;
    package[2] = Bms::kEmptybuffervalue;
    bus->PerformWriteTransaction(address, package, 3);

    package[0] = Bms::kJeitaT1RegisterLocation;
    package[1] = Bms::kJeitaT1ConfiqurationLBValue;
    package[2] = Bms::kJeitaT1ConfiqurationUBValue;
    bus->PerformWriteTransaction(address, package, 3);

    package[0] = Bms::kIChargeJeita5to6RegisterLocation;
    package[1] = Bms::kIChargeJeita5to6ConfigurationValue;
    package[2] = Bms::kEmptybuffervalue;
    bus->PerformWriteTransaction(address, package, 3);

    package[0] = Bms::kIChargeJeita2to4RegisterLocation;
    package[1] = Bms::kIChargeJeita2to4ConfigurationLBValue;
    package[2] = Bms::kIChargeJeita2to4ConfigurationUBValue;
    bus->PerformWriteTransaction(address, package, 3);

    package[0] = Bms::kCoulomConfigRegisterLocation;
    package[1] = Bms::kCoulomConfigRegisterValue;
    package[2] = Bms::kEmptybuffervalue;
    bus->PerformWriteTransaction(address, package, 3);

    package[0] = Bms::kQCountInitialRegisterLocation;
    package[1] = Bms::kQCountInitialLRegisterValue;
    package[2] = Bms::kQCountInitialURegisterValue;
    bus->PerformWriteTransaction(address, package, 3);

    package[0] = Bms::kPrescaleFactorRegisterLocation;
    package[1] = Bms::kPrescaleFactorRegisterValue;
    package[2] = Bms::kEmptybuffervalue;
    bus->PerformWriteTransaction(address, package, 3);
}

// Read 2-byte from BMS Registers
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

// BMS System Status
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

// Jeita Region VCharge
uint16_t Bms::GetJeitaRegionVCharge(etl::array<byte, 2>& read_buffer)
{
    if (GetTelemetryValid(read_buffer))
    {
        SelectRegister(kJeitaRegionRegisterLocation);
        ReadFromCurrentRegister(read_buffer);
        uint16_t Jeita_region_binary_reading;
        Jeita_region_binary_reading =
                ((static_cast<uint16_t>(read_buffer.at(1))) << 8)
                        | static_cast<uint16_t>(read_buffer.at(0));
        return Jeita_region_binary_reading;
    }
}
// VCharge Dec
double Bms::GetVChargeDEC(etl::array<byte, 2>& read_buffer)
{
    SelectRegister(kVChargeDACRegisterLocation);
    ReadFromCurrentRegister(read_buffer);
    uint16_t vcharge_dec_binary_reading =
            ((static_cast<uint16_t>(read_buffer.at(1))) << 8)
                    | static_cast<uint16_t>(read_buffer.at(0));
    return (vcharge_dec_binary_reading / kVchargeDivisionFactor)
            + kVchargeAdditionFactor;
}

// ICharge Dec
double Bms::GetIChargeDEC(etl::array<byte, 2>& read_buffer)
{
    SelectRegister(kIChargeDACRegisterLocation);
    ReadFromCurrentRegister(read_buffer);
    uint16_t icharge_dec_binary_reading =
            ((static_cast<uint16_t>(read_buffer.at(1))) << 8)
                    | static_cast<uint16_t>(read_buffer.at(0));
    return (icharge_dec_binary_reading + kIchargeAdditionFactor)
            * kIchargeMultiplicationFactor;
}

double Bms::TakeI2cDieTempReading()
{
    etl::array<byte, 2> read_buffer;
    if (GetTelemetryValid(read_buffer))
    {
        SelectRegister(kDieTempRegister);
        ReadFromCurrentRegister(read_buffer);
        return ConvertToDieTemperature(read_buffer);
    }
}

double Bms::ConvertToDieTemperature(etl::array<byte, 2> read_buffer)
{
    uint16_t register_value = (read_buffer[1] << 8) | read_buffer[0];
    double temp_in_celcius = (register_value - kDieTempOffset)
            / kDieTempConversionFactor;
    return temp_in_celcius;
}

double Bms::TakeI2cBatteryTempReading()
{
    etl::array<byte, 2> read_buffer;
    if (GetTelemetryValid(read_buffer))
    {
        SelectRegister(kNTCRatioRegister);
        ReadFromCurrentRegister(read_buffer);
        return ConvertToBatteryTemperature(read_buffer);
    }
}

double Bms::ConvertToBatteryTemperature(etl::array<byte, 2> read_buffer)
{
    uint16_t ntc_ratio_register_value = (read_buffer[1] << 8) | read_buffer[0];
    double rntc_resistance = kNTCBiasResistance * ntc_ratio_register_value
            / (kNTCBitWeight - ntc_ratio_register_value);
    double battery_temp_in_kelvin = 1
            / (kConversionCoefficientA
                    + kConversionCoefficientB * log(rntc_resistance)
                    + kConversionCoefficientC * pow(log(rntc_resistance), 3));
    return battery_temp_in_kelvin - kKelvinToCelciusOffset;
}

//Coulumb Counting
double Bms::GetSOCinPercent(etl::array<byte, 2> read_buffer)
{
    if (GetTelemetryValid(read_buffer))
    {
        Initial_Charge_complete = true;
        double q_count;
        double SOC;
        SelectRegister(kQCountInitialRegisterLocation);
        ReadFromCurrentRegister(read_buffer);
        uint16_t q_count_reading = ((static_cast<uint16_t>(read_buffer.at(1)))
                << 8) | static_cast<uint16_t>(read_buffer.at(0));

        if (Initial_Charge_complete)
        {
            q_count = q_count_reading - kQCountAtInitial;
            return SOC = kSOCPercentFactor * (q_count / kQCountMaximum);
        }
        else
        {
            return SOC = kSOCPercentFactor
                    * (q_count / kQCountAtHundreadPercent);
        }

        if (GetCOverXTerm(read_buffer))
        {
            q_count = kQCountAtHundreadPercent;
            if (Initial_Charge_complete)
                Initial_Charge_complete = false;
        }
    }
}

bool Bms::GetCOverXTerm(etl::array<byte, 2>& read_buffer)
{
    if (GetTelemetryValid(read_buffer))
    {
        SelectRegister(kChargeStateRegisterLocation);
        ReadFromCurrentRegister(read_buffer);
        uint16_t c_over_x_term_reading = ((static_cast<uint16_t>(read_buffer.at(
                1))) << 8) | static_cast<uint16_t>(read_buffer.at(0));
        if (c_over_x_term_reading & kCOverXTermBitMask)
            return true;
        else
            return false;
    }
}

