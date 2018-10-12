#include <math.h>
#include <src/board/i2c/bms/bms.h>
#include <src/board/i2c/i2c.h>

Bms::Bms(const I2c* bus, int address, const I2cMultiplexer* multiplexer,
         I2cMultiplexer::MuxChannel channel)
    : I2cDevice(bus, address, multiplexer, channel),
      first_charge_complete(false) {
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

/* get methods */
BmsOperationValuesReading Bms::GetBmsOperationValuesReading() {
    BmsOperationValuesReading bms_operation_values_reading;
    bms_operation_values_reading.c_over_x_threshold = GetCOverXThreshold();
    bms_operation_values_reading.recharge_threshold = GetRechargeThreshold();
    bms_operation_values_reading.q_count_prescale_factor =
        GetQCountPrescaleFactor();
    bms_operation_values_reading.timestamp_ms =
        BmsOperationValuesReading_timestamp_ms_default;
    return bms_operation_values_reading;
}

BmsSettingsReading Bms::GetBmsSettingsReading() {
    BmsSettingsReading bms_settings_reading;
    bms_settings_reading.charger_config = GetChargerConfig();
    bms_settings_reading.v_charge_setting = GetVchargeSetting();
    bms_settings_reading.i_charge_target = GetIchargeTarget();
    bms_settings_reading.config_bits = GetConfigBits();
    bms_settings_reading.v_in_uvcl_setting = GetVinUvclSetting();
    bms_settings_reading.telemetry_valid = GetTelemetryValid();
    bms_settings_reading.timestamp_ms = BmsSettingsReading_timestamp_ms_default;
    return bms_settings_reading;
}

BmsChargingInfoReading Bms::GetBmsChargingInfoReading() {
    BmsChargingInfoReading bms_charging_info_reading;
    bms_charging_info_reading.system_status = GetSystemStatus();
    bms_charging_info_reading.charge_status = GetChargeStatus();
    bms_charging_info_reading.charger_state = GetChargerState();
    bms_charging_info_reading.q_count = GetQCount();
    bms_charging_info_reading.q_count_delta = GetQCountDelta();
    bms_charging_info_reading.first_charge_complete = GetFirstChargeComplete();
    bms_charging_info_reading.timestamp_ms =
        BmsChargingInfoReading_timestamp_ms_default;
    return bms_charging_info_reading;
}

BmsTemperatureReading Bms::GetBmsTemperatureReading() {
    BmsTemperatureReading bms_temperature_reading;
    bms_temperature_reading.battery_temperature = GetBatteryTemp();
    bms_temperature_reading.die_temperature = GetDieTemp();
    bms_temperature_reading.jeita_region = GetJeitaRegion();
    bms_temperature_reading.timestamp_ms =
        BmsTemperatureReading_timestamp_ms_default;
    return bms_temperature_reading;
}

BmsVoltagesReading Bms::GetBmsVoltagesReading() {
    BmsVoltagesReading bms_voltages_reading;
    bms_voltages_reading.battery_voltage = GetBatteryVoltage();
    bms_voltages_reading.system_voltage = GetSystemVoltage();
    bms_voltages_reading.input_voltage = GetInputVoltage();
    bms_voltages_reading.timestamp_ms = BmsVoltagesReading_timestamp_ms_default;
    return bms_voltages_reading;
}

BmsCurrentsReading Bms::GetBmsCurrentsReading() {
    BmsCurrentsReading bms_currents_reading;
    bms_currents_reading.battery_current = GetBatteryCurrent();
    bms_currents_reading.input_current = GetInputCurrent();
    bms_currents_reading.timestamp_ms = BmsCurrentsReading_timestamp_ms_default;
    return bms_currents_reading;
}

double Bms::GetBatteryVoltage() {
    return GetFromRegisterAndConvert<double>(
        kVbatRegisterLocation, &ConvertToBatteryVoltage,
        BmsVoltagesReading_battery_voltage_default);
}

double Bms::GetBatteryCurrent() {
    return GetFromRegisterAndConvert<double>(
        kIbatRegisterLocation, &ConvertToBatteryCurrent,
        BmsCurrentsReading_battery_current_default);
}

double Bms::GetSystemVoltage() {
    return GetFromRegisterAndConvert<double>(
        kVsysRegisterLocation, &ConvertToSystemVoltage,
        BmsVoltagesReading_system_voltage_default);
}

double Bms::GetInputVoltage() {
    return GetFromRegisterAndConvert<double>(
        kVinRegisterLocation, &ConvertToInputVoltage,
        BmsVoltagesReading_input_voltage_default);
}

double Bms::GetInputCurrent() {
    return GetFromRegisterAndConvert<double>(
        kIinRegisterLocation, &ConvertToInputCurrent,
        BmsCurrentsReading_input_current_default);
}

double Bms::GetDieTemp() {
    return GetFromRegisterAndConvert<double>(
        kDieTempRegisterLocation, &ConvertToDieTemperature,
        BmsTemperatureReading_die_temperature_default);
}

double Bms::GetBatteryTemp() {
    return GetFromRegisterAndConvert<double>(
        kNtcRatioRegisterLocation, &ConvertToBatteryTemperature,
        BmsTemperatureReading_battery_temperature_default);
}

uint16_t Bms::GetJeitaRegion() {
    return GetFromRegisterAndConvert<uint16_t>(
        kJeitaRegionRegisterLocation, &ConvertToJeitaRegion,
        BmsTemperatureReading_jeita_region_default);
}

BmsChargingInfoReading_SystemStatus Bms::GetSystemStatus() {
    return GetFromRegisterAndConvert<BmsChargingInfoReading_SystemStatus>(
        kSystemStatusRegisterLocation, &ConvertToSystemStatus,
        BmsChargingInfoReading_system_status_default);
}

BmsChargingInfoReading_ChargerState Bms::GetChargerState() {
    return GetFromRegisterAndConvert<BmsChargingInfoReading_ChargerState>(
        kChargerStateRegisterLocation, &ConvertToChargerState,
        BmsChargingInfoReading_charger_state_default);
}

BmsChargingInfoReading_ChargeStatus Bms::GetChargeStatus() {
    return GetFromRegisterAndConvert<BmsChargingInfoReading_ChargeStatus>(
        kChargeStatusRegisterLocation, &ConvertToChargeStatus,
        BmsChargingInfoReading_charge_status_default);
}

double Bms::GetRechargeThreshold() {
    return GetFromRegisterAndConvert<double>(
        kRechargeThresholdRegisterLocation, &ConvertToRechargeThreshold,
        BmsOperationValuesReading_recharge_threshold_default);
}

uint16_t Bms::GetChargerConfig() {
    return GetFromRegisterAndConvert<uint16_t>(
        kChargerConfigBitsRegisterLocation, &ConvertToChargerConfig,
        BmsSettingsReading_charger_config_default);
}

uint16_t Bms::GetCOverXThreshold() {
    return GetFromRegisterAndConvert<uint16_t>(
        kCOverXThresholdRegisterLocation, &ConvertToCOverXThreshold,
        BmsOperationValuesReading_c_over_x_threshold_default);
}

uint16_t Bms::GetVchargeSetting() {
    return GetFromRegisterAndConvert<uint16_t>(
        kVchargeSettingRegisterLocation, &ConvertToVchargeSetting,
        BmsSettingsReading_v_charge_setting_default);
}

uint16_t Bms::GetIchargeTarget() {
    return GetFromRegisterAndConvert<uint16_t>(
        kIchargeTargetRegisterLocation, &ConvertToIchargeTarget,
        BmsSettingsReading_i_charge_target_default);
}

uint16_t Bms::GetConfigBits() {
    return GetFromRegisterAndConvert<uint16_t>(
        kConfigBitsRegisterLocation, &ConvertToConfigBits,
        BmsSettingsReading_config_bits_default);
}

uint16_t Bms::GetQCount() {
    return GetFromRegisterAndConvert<uint16_t>(
        kQCountRegisterLocation, &ConvertToQCount,
        BmsChargingInfoReading_q_count_default);
}

uint16_t Bms::GetQCountPrescaleFactor() {
    return GetFromRegisterAndConvert<uint16_t>(
        kQCountPrescaleFactorRegisterLocation, &ConvertToQCountPrescaleFactor,
        BmsOperationValuesReading_q_count_prescale_factor_default);
}

uint16_t Bms::GetVinUvclSetting() {
    return GetFromRegisterAndConvert<uint16_t>(
        kVinUvclSettingRegisterLocation, &ConvertToVinUvclSetting,
        BmsSettingsReading_v_in_uvcl_setting_default);
}

bool Bms::GetTelemetryValid() {
    return GetFromRegisterAndConvert<bool>(
        kMeasSysValidRegisterLocation, &ConvertToTelemetryValid,
        BmsSettingsReading_telemetry_valid_default);
}

bool Bms::GetFirstChargeComplete() { return first_charge_complete; }

int32_t Bms::GetQCountDelta() { return GetQCount() - kQCountInitial; }

/* conversion methods */
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

BmsChargingInfoReading_SystemStatus Bms::ConvertToSystemStatus(
    etl::array<byte, 2>& read_buffer) {
    uint16_t system_status_register_value =
        (read_buffer[1] << 8) | read_buffer[0];
    if (system_status_register_value & kChargeEnableBitMask) {
        return BmsChargingInfoReading_SystemStatus_kChargeEnable;
    } else {
        return BmsChargingInfoReading_SystemStatus_kChargeDisable;
    }
}

BmsChargingInfoReading_ChargerState Bms::ConvertToChargerState(
    etl::array<byte, 2>& read_buffer) {
    uint16_t charger_state_register_value =
        (read_buffer[1] << 8) | read_buffer[0];
    // TODO(hugorilla): handle error if cast doesn't work
    return static_cast<BmsChargingInfoReading_ChargerState>(
        charger_state_register_value);
}

BmsChargingInfoReading_ChargeStatus Bms::ConvertToChargeStatus(
    etl::array<byte, 2>& read_buffer) {
    uint16_t charge_status_register_value =
        (read_buffer[1] << 8) | read_buffer[0];
    if (charge_status_register_value & kConstantVoltageBitMask)
        return BmsChargingInfoReading_ChargeStatus_kConstantVoltage;
    else if (charge_status_register_value & kConstantCurrentBitMask)
        return BmsChargingInfoReading_ChargeStatus_kConstantCurrent;
    else if (charge_status_register_value & kIinLimitActiveBitMask)
        return BmsChargingInfoReading_ChargeStatus_kIinLimitActive;
    else if (charge_status_register_value & kVinLimitActiveBitMask)
        return BmsChargingInfoReading_ChargeStatus_kVinLimitActive;
    else if ((charge_status_register_value & kChargeStatusBitMask) ==
             kChargeStatusNotCharging)
        return BmsChargingInfoReading_ChargeStatus_kNotCharging;
    else
        return BmsChargingInfoReading_ChargeStatus_kChargingError;
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

bool Bms::ConvertToTelemetryValid(etl::array<byte, 2>& read_buffer) {
    uint16_t telemetry_system_register_value =
        (read_buffer[1] << 8) | read_buffer[0];
    if (telemetry_system_register_value & kTelemetryValidBitMask)
        return true;
    else
        return false;
}
