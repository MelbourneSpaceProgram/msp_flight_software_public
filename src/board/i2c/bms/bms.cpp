#include <math.h>
#include <src/board/i2c/bms/bms.h>
#include <src/board/i2c/i2c.h>

Bms::Bms(const I2c* bus, int address, const I2cMultiplexer* multiplexer,
         I2cMultiplexer::MuxChannel channel)
    : I2cDevice(bus, address, multiplexer, channel) {
    SetConfiguration();
}

void Bms::SetConfiguration() {
    WriteToRegister(kVchargeSettingRegisterLocation,
                    kVchargeSettingConfigurationValue, kEmptyBufferValue);

    WriteToRegister(kIchargeTargetRegisterLocation,
                    kIChargeTargetConfigurationValue, kEmptyBufferValue);

    WriteToRegister(kRechargeThresholdRegisterLocation,
                    kRechargeThresholdConfigurationLBValue,
                    kRechargeThresholdConfigurationUBValue);

    WriteToRegister(kMaxCvTimeRegisterLocation, kMaxCvTimeConfigurationValue,
                    kEmptyBufferValue);

    WriteToRegister(kChargerConfigBitsRegisterLocation,
                    kChargerConfigBitsConfigurationValue, kEmptyBufferValue);

    WriteToRegister(kCOverXThresholdRegisterLocation,
                    kCOverXThresholdConfigurationLBValue,
                    kCOverXThresholdConfigurationUBValue);

    WriteToRegister(kJeitaT1RegisterLocation, kJeitaT1ConfigurationLBValue,
                    kJeitaT1ConfigurationUBValue);

    WriteToRegister(kVchargeJeita5to6RegisterLocation,
                    kVchargeJeita5to6ConfigurationLBValue,
                    kVchargeJeita5to6ConfigurationUBValue);

    WriteToRegister(kVchargeJeita2to4RegisterLocation,
                    kVchargeJeita2to4ConfigurationLBValue,
                    kVchargeJeita2to4ConfigurationUBValue);

    WriteToRegister(kIchargeJeita5to6RegisterLocation,
                    kIchargeJeita5to6ConfigurationLBValue,
                    kIchargeJeita5to6ConfigurationUBValue);

    WriteToRegister(kIchargeJeita2to4RegisterLocation,
                    kIchargeJeita2to4ConfigurationLBValue,
                    kIchargeJeita2to4ConfigurationUBValue);

    WriteToRegister(kConfigBitsRegisterLocation,
                    kConfigBitsConfigurationLBValue,
                    kConfigBitsConfigurationUBValue);

    WriteToRegister(kQCountRegisterLocation,
                    kQCountRegisterConfigurationLBValue,
                    kQCountRegisterConfigurationUBValue);

    WriteToRegister(kQCountPrescaleFactorRegisterLocation,
                    kQCountPrescaleFactorConfigurationLBValue,
                    kQCountPrescaleFactorConfigurationUBValue);

    WriteToRegister(kVinUvclSettingRegisterLocation,
                    kVinUvclSettingConfigurationValue, kEmptyBufferValue);
}

bool Bms::WriteToRegister(byte register_location, byte lower_byte,
                          byte upper_byte) {
    byte package[3];
    package[0] = register_location;
    package[1] = lower_byte;
    package[2] = upper_byte;
    if (GetTelemetryValid() && PerformWriteTransaction(address, package, 3)) {
        return true;
    }
    return false;
}

bool Bms::SelectRegister(byte register_address) {
    if (PerformWriteTransaction(address, &register_address, 1)) {
        return true;
    }
    return false;
}

bool Bms::ReadFromCurrentRegister(etl::array<byte, 2>& read_buffer) {
    byte i2c_buffer[2];
    if (PerformReadTransaction(address, i2c_buffer, 2)) {
        read_buffer.at(0) = i2c_buffer[0];
        read_buffer.at(1) = i2c_buffer[1];
        return true;
    }
    return false;
}

// TODO(hugorilla): Remove casts if possible to include enums in nanopb message
BmsReadings Bms::GetBmsReadings() {
    BmsReadings bms_readings;
    bms_readings.battery_voltage = GetBatteryVoltage();
    bms_readings.battery_current = GetBatteryCurrent();
    bms_readings.system_voltage = GetSystemVoltage();
    bms_readings.input_voltage = GetInputVoltage();
    bms_readings.input_current = GetInputCurrent();
    bms_readings.die_temperature = GetDieTemp();
    bms_readings.battery_temperature = GetBatteryTemp();
    bms_readings.jeita_region = GetJeitaRegion();
    bms_readings.system_status = static_cast<uint16_t>(GetSystemStatus());
    bms_readings.charger_state = static_cast<uint16_t>(GetChargerState());
    bms_readings.charge_status = static_cast<uint16_t>(GetChargeStatus());
    bms_readings.recharge_threshold = GetRechargeThreshold();
    bms_readings.charger_config = GetChargerConfig();
    bms_readings.c_over_x_threshold = GetCOverXThreshold();
    bms_readings.v_charge_setting = GetVchargeSetting();
    bms_readings.i_charge_target = GetIchargeTarget();
    bms_readings.config_bits = static_cast<uint16_t>(GetConfigBits());
    bms_readings.q_count = GetQCount();
    bms_readings.q_count_prescale_factor = GetQCountPrescaleFactor();
    bms_readings.v_in_uvcl_setting = GetVinUvclSetting();
    bms_readings.v_charge_dac = GetVchargeDac();
    bms_readings.i_charge_dac = GetIchargeDac();
    bms_readings.telemetry_valid = GetTelemetryValid();
    return bms_readings;
}

double Bms::GetBatteryVoltage() {
    return GetFromRegisterAndConvert<double>(
        kVbatRegisterLocation, &ConvertToBatteryVoltage,
        BmsReadings_battery_voltage_default);
}

double Bms::GetBatteryCurrent() {
    return GetFromRegisterAndConvert<double>(
        kIbatRegisterLocation, &ConvertToBatteryCurrent,
        BmsReadings_battery_current_default);
}

double Bms::GetSystemVoltage() {
    return GetFromRegisterAndConvert<double>(
        kVsysRegisterLocation, &ConvertToSystemVoltage,
        BmsReadings_system_voltage_default);
}

double Bms::GetInputVoltage() {
    return GetFromRegisterAndConvert<double>(kVinRegisterLocation,
                                             &ConvertToInputVoltage,
                                             BmsReadings_input_voltage_default);
}

double Bms::GetInputCurrent() {
    return GetFromRegisterAndConvert<double>(kIinRegisterLocation,
                                             &ConvertToInputCurrent,
                                             BmsReadings_input_current_default);
}

double Bms::GetDieTemp() {
    return GetFromRegisterAndConvert<double>(
        kDieTempRegisterLocation, &ConvertToDieTemperature,
        BmsReadings_die_temperature_default);
}

double Bms::GetBatteryTemp() {
    return GetFromRegisterAndConvert<double>(
        kNtcRatioRegisterLocation, &ConvertToBatteryTemperature,
        BmsReadings_battery_temperature_default);
}

uint16_t Bms::GetJeitaRegion() {
    return GetFromRegisterAndConvert<uint16_t>(
        kJeitaRegionRegisterLocation, &ConvertToJeitaRegion,
        BmsReadings_jeita_region_default);
}

Bms::SystemStatus Bms::GetSystemStatus() {
    return GetFromRegisterAndConvert<Bms::SystemStatus>(
        kSystemStatusRegisterLocation, &ConvertToSystemStatus,
        static_cast<Bms::SystemStatus>(BmsReadings_system_status_default));
}

Bms::ChargerState Bms::GetChargerState() {
    return GetFromRegisterAndConvert<Bms::ChargerState>(
        kChargerStateRegisterLocation, &ConvertToChargerState,
        static_cast<Bms::ChargerState>(BmsReadings_charger_state_default));
}

Bms::ChargeStatus Bms::GetChargeStatus() {
    return GetFromRegisterAndConvert<Bms::ChargeStatus>(
        kChargeStatusRegisterLocation, &ConvertToChargeStatus,
        static_cast<Bms::ChargeStatus>(BmsReadings_charge_status_default));
}

double Bms::GetRechargeThreshold() {
    return GetFromRegisterAndConvert<double>(
        kRechargeThresholdRegisterLocation, &ConvertToRechargeThreshold,
        BmsReadings_recharge_threshold_default);
}

uint16_t Bms::GetChargerConfig() {
    return GetFromRegisterAndConvert<uint16_t>(
        kChargerConfigBitsRegisterLocation, &ConvertToChargerConfig,
        BmsReadings_charger_config_default);
}

uint16_t Bms::GetCOverXThreshold() {
    return GetFromRegisterAndConvert<uint16_t>(
        kCOverXThresholdRegisterLocation, &ConvertToCOverXThreshold,
        BmsReadings_c_over_x_threshold_default);
}

uint16_t Bms::GetVchargeSetting() {
    return GetFromRegisterAndConvert<uint16_t>(
        kVchargeSettingRegisterLocation, &ConvertToVchargeSetting,
        BmsReadings_v_charge_setting_default);
}

uint16_t Bms::GetIchargeTarget() {
    return GetFromRegisterAndConvert<uint16_t>(
        kIchargeTargetRegisterLocation, &ConvertToIchargeTarget,
        BmsReadings_i_charge_target_default);
}

uint16_t Bms::GetConfigBits() {
    return GetFromRegisterAndConvert<uint16_t>(kConfigBitsRegisterLocation,
                                               &ConvertToConfigBits,
                                               BmsReadings_config_bits_default);
}

uint16_t Bms::GetQCount() {
    return GetFromRegisterAndConvert<uint16_t>(
        kQCountRegisterLocation, &ConvertToQCount, BmsReadings_q_count_default);
}

uint16_t Bms::GetQCountPrescaleFactor() {
    return GetFromRegisterAndConvert<uint16_t>(
        kQCountPrescaleFactorRegisterLocation, &ConvertToQCountPrescaleFactor,
        BmsReadings_q_count_prescale_factor_default);
}

uint16_t Bms::GetVinUvclSetting() {
    return GetFromRegisterAndConvert<uint16_t>(
        kVinUvclSettingRegisterLocation, &ConvertToVinUvclSetting,
        BmsReadings_v_in_uvcl_setting_default);
}

double Bms::GetVchargeDac() {
    return GetFromRegisterAndConvert<double>(kVchargeDacRegisterLocation,
                                             &ConvertToVchargeDac,
                                             BmsReadings_v_charge_dac_default);
}

double Bms::GetIchargeDac() {
    return GetFromRegisterAndConvert<double>(kVchargeDacRegisterLocation,
                                             &ConvertToIchargeDac,
                                             BmsReadings_i_charge_dac_default);
}

bool Bms::GetTelemetryValid() {
    return GetFromRegisterAndConvert<bool>(kMeasSysValidRegisterLocation,
                                           &ConvertToTelemetryValid,
                                           BmsReadings_telemetry_valid_default);
}

double Bms::ConvertToBatteryVoltage(etl::array<byte, 2>& read_buffer) {
    int16_t v_bat_register_value =
        (read_buffer[1] << 8) | read_buffer[0];  // two's complement
    return v_bat_register_value * kLithiumBatteryVoltageConversionFactor;
}

double Bms::ConvertToBatteryCurrent(etl::array<byte, 2>& read_buffer) {
    int16_t i_bat_register_value =
        (read_buffer[1] << 8) | read_buffer[0];  // two's complement
    return i_bat_register_value * kBatteryCurrentConversionFactor;
}

double Bms::ConvertToSystemVoltage(etl::array<byte, 2>& read_buffer) {
    int16_t v_sys_register_value =
        (read_buffer[1] << 8) | read_buffer[0];  // two's complement
    return v_sys_register_value * kSystemVoltageConversionFactor;
}

double Bms::ConvertToInputVoltage(etl::array<byte, 2>& read_buffer) {
    int16_t v_in_register_value =
        (read_buffer[1] << 8) | read_buffer[0];  // two's complement
    return v_in_register_value * kInputVoltageConversionFactor;
}

double Bms::ConvertToInputCurrent(etl::array<byte, 2>& read_buffer) {
    uint16_t i_in_register_value = (read_buffer[1] << 8) | read_buffer[0];
    return i_in_register_value * kInputCurrentConversionFactor;
}

double Bms::ConvertToDieTemperature(etl::array<byte, 2>& read_buffer) {
    uint16_t die_temp_register_value = (read_buffer[1] << 8) | read_buffer[0];
    double temp_in_celcius =
        (die_temp_register_value - kDieTempOffset) / kDieTempConversionFactor;
    return temp_in_celcius;
}

double Bms::ConvertToBatteryTemperature(etl::array<byte, 2>& read_buffer) {
    uint16_t ntc_ratio_register_value = (read_buffer[1] << 8) | read_buffer[0];
    double rntc_resistance = kNtcBiasResistance * ntc_ratio_register_value /
                             (kNtcBitWeight - ntc_ratio_register_value);
    double battery_temp_in_kelvin =
        1 / (kConversionCoefficientA +
             kConversionCoefficientB * log(rntc_resistance) +
             kConversionCoefficientC * pow(log(rntc_resistance), 3));
    return battery_temp_in_kelvin - kKelvinToCelciusOffset;
}

uint16_t Bms::ConvertToJeitaRegion(etl::array<byte, 2>& read_buffer) {
    uint16_t jeita_region_register_value =
        (read_buffer[1] << 8) | read_buffer[0];
    return jeita_region_register_value & kJeitaRegionBitMask;
}

Bms::SystemStatus Bms::ConvertToSystemStatus(etl::array<byte, 2>& read_buffer) {
    uint16_t system_status_register_value =
        (read_buffer[1] << 8) | read_buffer[0];
    if (system_status_register_value & kChargeEnableBitMask) {
        return kChargeEnable;
    } else {
        return kChargeDisable;
    }
}

Bms::ChargerState Bms::ConvertToChargerState(etl::array<byte, 2>& read_buffer) {
    uint16_t charger_state_register_value =
        (read_buffer[1] << 8) | read_buffer[0];
    // TODO(hugorilla): perform check on charger_state_register value
    return static_cast<Bms::ChargerState>(charger_state_register_value);
}

Bms::ChargeStatus Bms::ConvertToChargeStatus(etl::array<byte, 2>& read_buffer) {
    uint16_t charge_status_register_value =
        (read_buffer[1] << 8) | read_buffer[0];
    if (charge_status_register_value & kConstantVoltageBitMask)
        return kConstantVoltage;
    else if (charge_status_register_value & kConstantCurrentBitMask)
        return kConstantCurrent;
    else if (charge_status_register_value & kIinLimitActiveBitMask)
        return kIinLimitActive;
    else if (charge_status_register_value & kVinLimitActiveBitMask)
        return kVinLimitActive;
    else if ((charge_status_register_value & kChargeStatusBitMask) ==
             kChargeStatusNotCharging)
        return kNotCharging;
    else
        return kError;
}

double Bms::ConvertToRechargeThreshold(etl::array<byte, 2>& read_buffer) {
    int16_t recharge_threshold_register_value =
        (read_buffer[1] << 8) | read_buffer[0];  // two's complement
    return recharge_threshold_register_value *
           kRechargeThresholdConversionFactor;
}

uint16_t Bms::ConvertToChargerConfig(etl::array<byte, 2>& read_buffer) {
    uint16_t charger_config_register_value =
        (read_buffer[1] << 8) | read_buffer[0];
    return charger_config_register_value & kChargerConfigBitMask;
}

uint16_t Bms::ConvertToCOverXThreshold(etl::array<byte, 2>& read_buffer) {
    int16_t c_x_threshold_register_value =
        (read_buffer[1] << 8) | read_buffer[0];  // two's complement
    return c_x_threshold_register_value;
}

uint16_t Bms::ConvertToVchargeSetting(etl::array<byte, 2>& read_buffer) {
    uint16_t charge_voltage_setting_register_value =
        (read_buffer[1] << 8) | read_buffer[0];
    return charge_voltage_setting_register_value & kVchargeSettingBitMask;
}

uint16_t Bms::ConvertToIchargeTarget(etl::array<byte, 2>& read_buffer) {
    uint16_t charge_current_target_register_value =
        (read_buffer[1] << 8) | read_buffer[0];
    return charge_current_target_register_value & kIchargeTargetBitMask;
}

uint16_t Bms::ConvertToConfigBits(etl::array<byte, 2>& read_buffer) {
    uint16_t configuration_bits_register_value =
        (read_buffer[1] << 8) | read_buffer[0];
    return configuration_bits_register_value;
}

uint16_t Bms::ConvertToQCount(etl::array<byte, 2>& read_buffer) {
    uint16_t q_count_register_value = (read_buffer[1] << 8) | read_buffer[0];
    return q_count_register_value;
}

uint16_t Bms::ConvertToQCountPrescaleFactor(etl::array<byte, 2>& read_buffer) {
    uint16_t q_count_prescale_factor_register_value =
        (read_buffer[1] << 8) | read_buffer[0];
    return q_count_prescale_factor_register_value;
}

uint16_t Bms::ConvertToVinUvclSetting(etl::array<byte, 2>& read_buffer) {
    uint16_t v_in_uvcl_register_value = (read_buffer[1] << 8) | read_buffer[0];
    return v_in_uvcl_register_value;
}

double Bms::ConvertToVchargeDac(etl::array<byte, 2>& read_buffer) {
    uint16_t v_charge_dac_register_value =
        (read_buffer[1] << 8) | read_buffer[0];
    return (v_charge_dac_register_value / kVchargeDivisionFactor) +
           kVchargeAdditionFactor;
}

double Bms::ConvertToIchargeDac(etl::array<byte, 2>& read_buffer) {
    uint16_t i_charge_dac_register_value =
        (read_buffer[1] << 8) | read_buffer[0];
    return (i_charge_dac_register_value + kIchargeAdditionFactor) *
           kIchargeMultiplicationFactor;
}

bool Bms::ConvertToTelemetryValid(etl::array<byte, 2>& read_buffer) {
    uint16_t telemetry_system_register_value =
        (read_buffer[1] << 8) | read_buffer[0];
    if (telemetry_system_register_value & kTelemetryValidBitMask)
        return true;
    else
        return false;
}
