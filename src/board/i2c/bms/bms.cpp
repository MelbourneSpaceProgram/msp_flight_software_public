#include <math.h>
#include <src/board/i2c/bms/bms.h>
#include <src/board/i2c/i2c.h>

Bms::Bms(const I2c* bus, int address, const I2cMultiplexer* multiplexer,
         I2cMultiplexer::MuxChannel channel)
    : I2cDevice(bus, address, multiplexer, channel) {
    SetConfiguration();
}

void Bms::SetConfiguration() {
    byte package[3];

    package[0] = Bms::kVChargeSettingRegisterLocation;
    package[1] = Bms::kVChargeSettingRegisterValue;
    package[2] = Bms::kEmptybuffervalue;
    PerformWriteTransaction(address, package, 3);

    package[0] = Bms::kReChargeThresholdRegisterLocation;
    package[1] = Bms::kReChargeThresholdLRegisterValue;
    package[2] = Bms::kReChargeThresholdURegisterValue;
    PerformWriteTransaction(address, package, 3);

    package[0] = Bms::kMaxCVTimeRegisterLocation;
    package[1] = Bms::kMaxCVTimeRegisterValue;
    package[2] = Bms::kEmptybuffervalue;
    PerformWriteTransaction(address, package, 3);

    package[0] = Bms::kCXJeitaEnableRegisterLocation;
    package[1] = Bms::kCXJeitaEnableRegisterValue;
    package[2] = Bms::kEmptybuffervalue;
    PerformWriteTransaction(address, package, 3);

    package[0] = Bms::kCXThresholdRegisterLocation;
    package[1] = Bms::kCXThresholdRegisterValue;
    package[2] = Bms::kEmptybuffervalue;
    PerformWriteTransaction(address, package, 3);

    package[0] = Bms::kJeitaT1RegisterLocation;
    package[1] = Bms::kJeitaT1ConfiqurationLBValue;
    package[2] = Bms::kJeitaT1ConfiqurationUBValue;
    PerformWriteTransaction(address, package, 3);

    package[0] = Bms::kVChargeJeita5to6RegisterLocation;
    package[1] = Bms::kVChargeJeita5to6ConfigurationLBValue;
    package[2] = Bms::kVChargeJeita5to6ConfigurationUBValue;
    PerformWriteTransaction(address, package, 3);

    package[0] = Bms::kVChargeJeita2to4RegisterLocation;
    package[1] = Bms::kVChargeJeita2to4ConfigurationLBValue;
    package[2] = Bms::kVChargeJeita2to4ConfigurationUBValue;
    PerformWriteTransaction(address, package, 3);

    package[0] = Bms::kIChargeJeita5to6RegisterLocation;
    package[1] = Bms::kIChargeJeita5to6ConfigurationLBValue;
    package[2] = Bms::kIChargeJeita5to6ConfigurationUBValue;
    PerformWriteTransaction(address, package, 3);

    package[0] = Bms::kIChargeJeita2to4RegisterLocation;
    package[1] = Bms::kIChargeJeita2to4ConfigurationLBValue;
    package[2] = Bms::kIChargeJeita2to4ConfigurationUBValue;
    PerformWriteTransaction(address, package, 3);

    package[0] = Bms::kCoulombConfigRegisterLocation;
    package[1] = Bms::kCoulombConfigRegisterInitialValue;
    package[2] = Bms::kEmptybuffervalue;
    PerformWriteTransaction(address, package, 3);

    package[0] = Bms::kQCountRegisterLocation;
    package[1] = Bms::kQCountRegisterInitialValue;
    package[2] = Bms::kEmptybuffervalue;
    PerformWriteTransaction(address, package, 3);

    package[0] = Bms::kQCountPrescaleFactorRegisterLocation;
    package[1] = Bms::kQCountPrescaleFactorRegisterInitialValue;
    package[2] = Bms::kEmptybuffervalue;
    PerformWriteTransaction(address, package, 3);
}

void Bms::SelectRegister(byte register_address) {
    PerformWriteTransaction(address, &register_address, 1);
}

void Bms::ReadFromCurrentRegister(etl::array<byte, 2>& read_buffer) {
    byte i2c_buffer[2];
    if (PerformReadTransaction(address, i2c_buffer, 2)) {
        read_buffer.at(0) = i2c_buffer[0];
        read_buffer.at(1) = i2c_buffer[1];
    }
}

// Jeita Region VCharge
uint16_t Bms::GetJeitaRegionVCharge(etl::array<byte, 2>& read_buffer) {
    if (GetTelemetryValid(read_buffer)) {
        SelectRegister(kJeitaRegionRegisterLocation);
        ReadFromCurrentRegister(read_buffer);
        uint16_t jeita_region_binary_reading;
        jeita_region_binary_reading =
            ((static_cast<uint16_t>(read_buffer.at(1))) << 8) |
            static_cast<uint16_t>(read_buffer.at(0));
        return jeita_region_binary_reading;
    }
    return kInvalidPositiveInteger;
}
// VCharge Dec
double Bms::GetVChargeDEC(etl::array<byte, 2>& read_buffer) {
    SelectRegister(kVChargeDACRegisterLocation);
    ReadFromCurrentRegister(read_buffer);
    uint16_t vcharge_dec_binary_reading =
        ((static_cast<uint16_t>(read_buffer.at(1))) << 8) |
        static_cast<uint16_t>(read_buffer.at(0));
    return (vcharge_dec_binary_reading / kVchargeDivisionFactor) +
           kVchargeAdditionFactor;
}

// ICharge Dec
double Bms::GetIChargeDEC(etl::array<byte, 2>& read_buffer) {
    SelectRegister(kIChargeDACRegisterLocation);
    ReadFromCurrentRegister(read_buffer);
    uint16_t icharge_dec_binary_reading =
        ((static_cast<uint16_t>(read_buffer.at(1))) << 8) |
        static_cast<uint16_t>(read_buffer.at(0));
    return (icharge_dec_binary_reading + kIchargeAdditionFactor) *
           kIchargeMultiplicationFactor;
}

// TODO(hugorilla): Throw errors in all TakeI2cReading methods if telemetry is
// not valid

BmsReadings Bms::GetBmsReadings() {
    etl::array<byte, 2> read_buffer;
    BmsReadings bms_readings;
    bms_readings.battery_voltage = GetBatteryVoltage();
    bms_readings.battery_current = GetBatteryCurrent();
    bms_readings.system_voltage = GetSystemVoltage();
    bms_readings.input_voltage = GetInputVoltage();
    bms_readings.input_current = GetInputCurrent();
    bms_readings.die_temperature = GetDieTemp();
    bms_readings.battery_temperature = GetBatteryTemp();
    bms_readings.jeita_region = GetJeitaRegion();
    // TODO(hugorilla) Update this method
    bms_readings.system_status =
        static_cast<uint16_t>(GetSystemStatus(read_buffer));
    // TODO(hugorilla) Update this method
    bms_readings.charger_state = static_cast<uint16_t>(GetChargerState());
    // TODO(hugorilla) Update this name (to be consistent with data sheet)
    bms_readings.charge_indicator =
        static_cast<uint16_t>(GetChargeStatus(read_buffer));
    bms_readings.recharge_threshold = GetRechargeThreshold();
    bms_readings.charger_config = GetChargerConfig();
    bms_readings.c_x_threshold = GetCXThreshold();
    bms_readings.v_charge_setting = GetChargeVoltageSetting();
    bms_readings.i_charge_target = GetChargeCurrentTarget();
    // TODO(hugorilla) Update this method
    bms_readings.configuration = static_cast<uint16_t>(
        GetConfiguration(kConfigurationBitsRegisterLocation, read_buffer));
    bms_readings.q_count = GetQCount();
    // TODO(hugorilla) Update this field name
    bms_readings.q_count_scale_factor = GetQCountPrescaleFactor();
    // TODO(hugorilla) Update this method
    bms_readings.telemetry_valid = GetTelemetryValid(read_buffer);
    return bms_readings;
}

double Bms::GetBatteryVoltage() {
    etl::array<byte, 2> read_buffer;
    if (GetTelemetryValid(read_buffer)) {
        SelectRegister(kVbatRegisterLocation);
        ReadFromCurrentRegister(read_buffer);
        return ConvertToBatteryVoltage(read_buffer);
    }
    return kInvalidDouble;
}

double Bms::ConvertToBatteryVoltage(etl::array<byte, 2>& read_buffer) {
    int16_t v_bat_register =
        (read_buffer.at(1) << 8) | read_buffer.at(0);  // two's complement
    return v_bat_register * kLithiumBatteryVoltageConversionFactor;
}

double Bms::GetBatteryCurrent() {
    etl::array<byte, 2> read_buffer;
    if (GetTelemetryValid(read_buffer)) {
        SelectRegister(kIbatRegisterLocation);
        ReadFromCurrentRegister(read_buffer);
        return ConvertToBatteryCurrent(read_buffer);
    }
    return kInvalidDouble;
}

double Bms::ConvertToBatteryCurrent(etl::array<byte, 2>& read_buffer) {
    int16_t i_bat_register =
        (read_buffer.at(1) << 8) | read_buffer.at(0);  // two's complement
    return i_bat_register * kBatteryCurrentConversionFactor;
}

double Bms::GetSystemVoltage() {
    etl::array<byte, 2> read_buffer;
    if (GetTelemetryValid(read_buffer)) {
        SelectRegister(kVsysRegisterLocation);
        ReadFromCurrentRegister(read_buffer);
        return ConvertToSystemVoltage(read_buffer);
    }
    return kInvalidDouble;
}

double Bms::ConvertToSystemVoltage(etl::array<byte, 2>& read_buffer) {
    int16_t v_sys_register =
        (read_buffer.at(1) << 8) | read_buffer.at(0);  // two's complement
    return v_sys_register * kSystemVoltageConversionFactor;
}

double Bms::GetInputVoltage() {
    etl::array<byte, 2> read_buffer;
    if (GetTelemetryValid(read_buffer)) {
        SelectRegister(kVinRegisterLocation);
        ReadFromCurrentRegister(read_buffer);
        return ConvertToInputVoltage(read_buffer);
    }
    return kInvalidDouble;
}

double Bms::ConvertToInputVoltage(etl::array<byte, 2>& read_buffer) {
    int16_t v_in_register =
        (read_buffer.at(1) << 8) | read_buffer.at(0);  // two's complement
    return v_in_register * kInputVoltageConversionFactor;
}

double Bms::GetInputCurrent() {
    etl::array<byte, 2> read_buffer;
    if (GetTelemetryValid(read_buffer)) {
        SelectRegister(kIinRegisterLocation);
        ReadFromCurrentRegister(read_buffer);
        return ConvertToInputCurrent(read_buffer);
    }
    return kInvalidDouble;
}

double Bms::ConvertToInputCurrent(etl::array<byte, 2>& read_buffer) {
    uint16_t i_in_register = (read_buffer.at(1) << 8) | read_buffer.at(0);
    return i_in_register * kInputCurrentConversionFactor;
}

double Bms::GetDieTemp() {
    etl::array<byte, 2> read_buffer;
    if (GetTelemetryValid(read_buffer)) {
        SelectRegister(kDieTempRegister);
        ReadFromCurrentRegister(read_buffer);
        return ConvertToDieTemperature(read_buffer);
    }
    return kInvalidDouble;
}

double Bms::ConvertToDieTemperature(etl::array<byte, 2>& read_buffer) {
    uint16_t register_value = (read_buffer.at(1) << 8) | read_buffer.at(0);
    double temp_in_celcius =
        (register_value - kDieTempOffset) / kDieTempConversionFactor;
    return temp_in_celcius;
}

double Bms::GetBatteryTemp() {
    etl::array<byte, 2> read_buffer;
    if (GetTelemetryValid(read_buffer)) {
        SelectRegister(kNTCRatioRegister);
        ReadFromCurrentRegister(read_buffer);
        return ConvertToBatteryTemperature(read_buffer);
    }
    return kInvalidDouble;
}

double Bms::ConvertToBatteryTemperature(etl::array<byte, 2>& read_buffer) {
    uint16_t ntc_ratio_register_value =
        (read_buffer.at(1) << 8) | read_buffer.at(0);
    double rntc_resistance = kNTCBiasResistance * ntc_ratio_register_value /
                             (kNTCBitWeight - ntc_ratio_register_value);
    double battery_temp_in_kelvin =
        1 / (kConversionCoefficientA +
             kConversionCoefficientB * log(rntc_resistance) +
             kConversionCoefficientC * pow(log(rntc_resistance), 3));
    return battery_temp_in_kelvin - kKelvinToCelciusOffset;
}

uint16_t Bms::GetJeitaRegion() {
    etl::array<byte, 2> read_buffer;
    if (GetTelemetryValid(read_buffer)) {
        SelectRegister(kJeitaRegionRegisterLocation);
        ReadFromCurrentRegister(read_buffer);
        return ConvertToJeitaRegion(read_buffer);
    }
    return kInvalidPositiveInteger;
}

uint16_t Bms::ConvertToJeitaRegion(etl::array<byte, 2>& read_buffer) {
    uint16_t jeita_region_register =
        (read_buffer.at(1) << 8) | read_buffer.at(0);
    return jeita_region_register & kJeitaRegionBitMask;
}

// BMS System Status
// TODO(hugorilla): update this method
Bms::SystemStatus Bms::GetSystemStatus(etl::array<byte, 2>& read_buffer) {
    if (GetTelemetryValid(read_buffer)) {
        SelectRegister(kSystemStatusRegisterLocation);
        ReadFromCurrentRegister(read_buffer);
        uint16_t system_status_binary_reading =
            ((static_cast<uint16_t>(read_buffer.at(1))) << 8) |
            static_cast<uint16_t>(read_buffer.at(0));
        if ((system_status_binary_reading & kChargeEnableBitMask) ==
            kChargeEnableBitMask)
            return kChargeEnable;
        else if ((system_status_binary_reading & kChargeEnableBitMask) !=
                 kChargeEnableBitMask)
            return kChargeDisable;
    }
    return kOther;
}

Bms::ChargerStates Bms::GetChargerState() {
    etl::array<byte, 2> read_buffer;
    if (GetTelemetryValid(read_buffer)) {
        SelectRegister(kChargerStateRegisterLocation);
        ReadFromCurrentRegister(read_buffer);
        return ConvertToChargerState(read_buffer);
    }
    return kInvalidChargerState;
}

Bms::ChargerStates Bms::ConvertToChargerState(
    etl::array<byte, 2>& read_buffer) {
    uint16_t charger_state_register =
        (read_buffer.at(1) << 8) | read_buffer.at(0);
    // TODO(hugorilla): perform check on charger_state_register value
    return static_cast<Bms::ChargerStates>(charger_state_register);
}

// BMS Charge Status
// TODO(hugorilla): Update this method
Bms::ChargeStatus Bms::GetChargeStatus(etl::array<byte, 2>& read_buffer) {
    if (GetTelemetryValid(read_buffer)) {
        SelectRegister(kChargeStatusRegisterLocation);
        ReadFromCurrentRegister(read_buffer);
        uint16_t charge_status_binary_reading;
        charge_status_binary_reading =
            ((static_cast<uint16_t>(read_buffer.at(1))) << 8) |
            static_cast<uint16_t>(read_buffer.at(0));
        if (charge_status_binary_reading & kConstantVoltageBitMask)
            return kConstantVoltage;
        else if (charge_status_binary_reading & kConstantCurrentBitMask)
            return kConstantCurrent;
        else if (charge_status_binary_reading & kIinLimitActiveBitMask)
            return kIinLimitActive;
        else if (charge_status_binary_reading & kVinLimitActiveBitMask)
            return kVinLimitActive;
        else if ((charge_status_binary_reading & kChargeStatusBitMask) ==
                 kChargeStatusNotCharging)
            return kNotCharging;
    }
    return kError;
}

double Bms::GetRechargeThreshold() {
    etl::array<byte, 2> read_buffer;
    if (GetTelemetryValid(read_buffer)) {
        SelectRegister(kReChargeThresholdRegisterLocation);
        ReadFromCurrentRegister(read_buffer);
        return ConvertToRechargeThreshold(read_buffer);
    }
    return kInvalidDouble;
}

double Bms::ConvertToRechargeThreshold(etl::array<byte, 2>& read_buffer) {
    int16_t recharge_threshold_register =
        (read_buffer.at(1) << 8) | read_buffer.at(0);  // two's complement
    return recharge_threshold_register * kRechargeThresholdConversionFactor;
}

uint16_t Bms::GetChargerConfig() {
    etl::array<byte, 2> read_buffer;
    if (GetTelemetryValid(read_buffer)) {
        SelectRegister(kChargerConfigRegisterLocation);
        ReadFromCurrentRegister(read_buffer);
        return ConvertToChargerConfig(read_buffer);
    }
    return kInvalidDouble;
}

uint16_t Bms::ConvertToChargerConfig(etl::array<byte, 2>& read_buffer) {
    uint16_t charger_config_register =
        (read_buffer.at(1) << 8) | read_buffer.at(0);
    return charger_config_register & kChargerConfigBitMask;
}

uint16_t Bms::GetCXThreshold() {
    etl::array<byte, 2> read_buffer;
    if (GetTelemetryValid(read_buffer)) {
        SelectRegister(kCXThresholdRegisterLocation);
        ReadFromCurrentRegister(read_buffer);
        return ConvertToCXThreshold(read_buffer);
    }
    return kInvalidPositiveInteger;
}

uint16_t Bms::ConvertToCXThreshold(etl::array<byte, 2>& read_buffer) {
    // TODO(hugorilla): determine whether this is stored in two's
    // complement; the data sheet isn't clear
    int16_t c_x_threshold_register =
        (read_buffer.at(1) << 8) | read_buffer.at(0);
    return c_x_threshold_register;
}

uint16_t Bms::GetChargeVoltageSetting() {
    etl::array<byte, 2> read_buffer;
    if (GetTelemetryValid(read_buffer)) {
        SelectRegister(kVChargeSettingRegisterLocation);
        ReadFromCurrentRegister(read_buffer);
        return ConvertToChargeVoltageSetting(read_buffer);
    }
    return kInvalidPositiveInteger;
}

uint16_t Bms::ConvertToChargeVoltageSetting(etl::array<byte, 2>& read_buffer) {
    uint16_t charge_voltage_setting_register =
        (read_buffer.at(1) << 8) | read_buffer.at(0);
    return charge_voltage_setting_register & kChargeVoltageSettingBitMask;
}

uint16_t Bms::GetChargeCurrentTarget() {
    etl::array<byte, 2> read_buffer;
    if (GetTelemetryValid(read_buffer)) {
        SelectRegister(kIChargeTargetRegisterLocation);
        ReadFromCurrentRegister(read_buffer);
        return ConvertToChargeCurrentTarget(read_buffer);
    }
    return kInvalidDouble;
}

uint16_t Bms::ConvertToChargeCurrentTarget(etl::array<byte, 2>& read_buffer) {
    uint16_t charge_current_target_register =
        (read_buffer.at(1) << 8) | read_buffer.at(0);
    return charge_current_target_register & kChargeCurrentTargetBitMask;
}

// Read 2-byte from BMS Registers
// TODO(hugorilla): update this method
uint16_t Bms::GetConfiguration(byte register_location,
                               etl::array<byte, 2>& read_buffer) {
    SelectRegister(register_location);
    ReadFromCurrentRegister(read_buffer);
    uint16_t read_binary_reading =
        ((static_cast<uint16_t>(read_buffer.at(1))) << 8) |
        static_cast<uint16_t>(read_buffer.at(0));
    return read_binary_reading;
}

uint16_t Bms::GetQCount() {
    etl::array<byte, 2> read_buffer;
    if (GetTelemetryValid(read_buffer)) {
        SelectRegister(kQCountRegisterLocation);
        ReadFromCurrentRegister(read_buffer);
        return ConvertToQCount(read_buffer);
    }
    return kInvalidPositiveInteger;
}

uint16_t Bms::ConvertToQCount(etl::array<byte, 2>& read_buffer) {
    uint16_t q_count_register = (read_buffer.at(1) << 8) | read_buffer.at(0);
    return q_count_register;
}

uint16_t Bms::GetQCountPrescaleFactor() {
    etl::array<byte, 2> read_buffer;
    if (GetTelemetryValid(read_buffer)) {
        SelectRegister(kQCountPrescaleFactorRegisterLocation);
        ReadFromCurrentRegister(read_buffer);
        return ConvertToQCountPrescaleFactor(read_buffer);
    }
    return kInvalidPositiveInteger;
}

uint16_t Bms::ConvertToQCountPrescaleFactor(etl::array<byte, 2>& read_buffer) {
    uint16_t q_count_prescale_factor_register =
        (read_buffer.at(1) << 8) | read_buffer.at(0);
    return q_count_prescale_factor_register;
}

// Checking valid condition of BMS' Telemetry system
bool Bms::GetTelemetryValid(etl::array<byte, 2>& read_buffer) {
    SelectRegister(kTelemetryValidRegisterLocation);
    ReadFromCurrentRegister(read_buffer);
    uint16_t telemetry_system_binary_reading =
        ((static_cast<uint16_t>(read_buffer.at(1))) << 8) |
        static_cast<uint16_t>(read_buffer.at(0));
    if (telemetry_system_binary_reading & kTelemetryValidBitMask)
        return true;
    else
        return false;
}
