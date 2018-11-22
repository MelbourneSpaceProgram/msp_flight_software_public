#include <CppUTest/TestHarness.h>
#include <external/etl/array.h>
#include <src/board/board.h>
#include <src/board/i2c/bms/bms.h>
#include <src/board/i2c/i2c.h>
#include <src/config/unit_tests.h>
#include <src/messages/TemperatureReading.pb.h>
#include <src/sensors/measurable_manager.h>

TEST_GROUP(Bms) {
    void setup() {
        if (!kBmsAvailable) {
            TEST_EXIT;
        }
    };
};

TEST(Bms, TestBmsChargingInfoRead) {
    MeasurableManager* measurable_manager = MeasurableManager::GetInstance();
    BmsChargingInfoReading bms_charging_info_reading_1 =
        measurable_manager->ReadNanopbMeasurable<BmsChargingInfoReading>(
            kEpsBmsChargingInfoReading1, 1);
    BmsChargingInfoReading bms_charging_info_reading_2 =
        measurable_manager->ReadNanopbMeasurable<BmsChargingInfoReading>(
            kEpsBmsChargingInfoReading2, 1);

    // BMS 1 tests
    CHECK(bms_charging_info_reading_1.system_status ==
              BmsChargingInfoReading_SystemStatus_kChargeDisable ||
          bms_charging_info_reading_1.system_status ==
              BmsChargingInfoReading_SystemStatus_kChargeEnable);

    CHECK_FALSE(bms_charging_info_reading_1.charger_state ==
                BmsChargingInfoReading_charger_state_default);
    CHECK(bms_charging_info_reading_1.charger_state ==
              BmsChargingInfoReading_ChargerState_kBigShortFault ||
          bms_charging_info_reading_1.charger_state ==
              BmsChargingInfoReading_ChargerState_kBatMissingFault ||
          bms_charging_info_reading_1.charger_state ==
              BmsChargingInfoReading_ChargerState_kMaxChargeTimeFault ||
          bms_charging_info_reading_1.charger_state ==
              BmsChargingInfoReading_ChargerState_kCOverXTerm ||
          bms_charging_info_reading_1.charger_state ==
              BmsChargingInfoReading_ChargerState_kTimerTerm ||
          bms_charging_info_reading_1.charger_state ==
              BmsChargingInfoReading_ChargerState_kNtcPause ||
          bms_charging_info_reading_1.charger_state ==
              BmsChargingInfoReading_ChargerState_kCcCvCharge ||
          bms_charging_info_reading_1.charger_state ==
              BmsChargingInfoReading_ChargerState_kPrecharge ||
          bms_charging_info_reading_1.charger_state ==
              BmsChargingInfoReading_ChargerState_kChargerSuspended ||
          bms_charging_info_reading_1.charger_state ==
              BmsChargingInfoReading_ChargerState_kAbsorbCharge ||
          bms_charging_info_reading_1.charger_state ==
              BmsChargingInfoReading_ChargerState_kEqualizeCharge ||
          bms_charging_info_reading_1.charger_state ==
              BmsChargingInfoReading_ChargerState_kInvalidChargerState);

    CHECK_FALSE(bms_charging_info_reading_1.charge_status ==
                BmsChargingInfoReading_charge_status_default);
    CHECK(bms_charging_info_reading_1.charge_status ==
              BmsChargingInfoReading_ChargeStatus_kConstantVoltage ||
          bms_charging_info_reading_1.charge_status ==
              BmsChargingInfoReading_ChargeStatus_kConstantCurrent ||
          bms_charging_info_reading_1.charge_status ==
              BmsChargingInfoReading_ChargeStatus_kIinLimitActive ||
          bms_charging_info_reading_1.charge_status ==
              BmsChargingInfoReading_ChargeStatus_kVinLimitActive ||
          bms_charging_info_reading_1.charge_status ==
              BmsChargingInfoReading_ChargeStatus_kNotCharging ||
          bms_charging_info_reading_1.charge_status ==
              BmsChargingInfoReading_ChargeStatus_kChargingError);

    CHECK_FALSE(bms_charging_info_reading_1.q_count ==
                BmsChargingInfoReading_q_count_default);
    CHECK(bms_charging_info_reading_1.q_count -
              bms_charging_info_reading_1.q_count_delta ==
          Bms::kQCountInitial);

    CHECK_FALSE(bms_charging_info_reading_1.timestamp_ms ==
                BmsChargingInfoReading_timestamp_ms_default);

    // BMS 2 tests
    CHECK(bms_charging_info_reading_2.system_status ==
              BmsChargingInfoReading_SystemStatus_kChargeDisable ||
          bms_charging_info_reading_2.system_status ==
              BmsChargingInfoReading_SystemStatus_kChargeEnable);

    CHECK_FALSE(bms_charging_info_reading_2.charger_state ==
                BmsChargingInfoReading_charger_state_default);
    CHECK(bms_charging_info_reading_2.charger_state ==
              BmsChargingInfoReading_ChargerState_kBigShortFault ||
          bms_charging_info_reading_2.charger_state ==
              BmsChargingInfoReading_ChargerState_kBatMissingFault ||
          bms_charging_info_reading_2.charger_state ==
              BmsChargingInfoReading_ChargerState_kMaxChargeTimeFault ||
          bms_charging_info_reading_2.charger_state ==
              BmsChargingInfoReading_ChargerState_kCOverXTerm ||
          bms_charging_info_reading_2.charger_state ==
              BmsChargingInfoReading_ChargerState_kTimerTerm ||
          bms_charging_info_reading_2.charger_state ==
              BmsChargingInfoReading_ChargerState_kNtcPause ||
          bms_charging_info_reading_2.charger_state ==
              BmsChargingInfoReading_ChargerState_kCcCvCharge ||
          bms_charging_info_reading_2.charger_state ==
              BmsChargingInfoReading_ChargerState_kPrecharge ||
          bms_charging_info_reading_2.charger_state ==
              BmsChargingInfoReading_ChargerState_kChargerSuspended ||
          bms_charging_info_reading_2.charger_state ==
              BmsChargingInfoReading_ChargerState_kAbsorbCharge ||
          bms_charging_info_reading_2.charger_state ==
              BmsChargingInfoReading_ChargerState_kEqualizeCharge ||
          bms_charging_info_reading_2.charger_state ==
              BmsChargingInfoReading_ChargerState_kInvalidChargerState);

    CHECK_FALSE(bms_charging_info_reading_2.charge_status ==
                BmsChargingInfoReading_charge_status_default);
    CHECK(bms_charging_info_reading_2.charge_status ==
              BmsChargingInfoReading_ChargeStatus_kConstantVoltage ||
          bms_charging_info_reading_2.charge_status ==
              BmsChargingInfoReading_ChargeStatus_kConstantCurrent ||
          bms_charging_info_reading_2.charge_status ==
              BmsChargingInfoReading_ChargeStatus_kIinLimitActive ||
          bms_charging_info_reading_2.charge_status ==
              BmsChargingInfoReading_ChargeStatus_kVinLimitActive ||
          bms_charging_info_reading_2.charge_status ==
              BmsChargingInfoReading_ChargeStatus_kNotCharging ||
          bms_charging_info_reading_2.charge_status ==
              BmsChargingInfoReading_ChargeStatus_kChargingError);

    CHECK_FALSE(bms_charging_info_reading_2.q_count ==
                BmsChargingInfoReading_q_count_default);
    CHECK(bms_charging_info_reading_2.q_count -
              bms_charging_info_reading_2.q_count_delta ==
          Bms::kQCountInitial);

    CHECK_FALSE(bms_charging_info_reading_2.timestamp_ms ==
                BmsChargingInfoReading_timestamp_ms_default);
}

TEST(Bms, TestBmsCurrentsReading) {
    MeasurableManager* measurable_manager = MeasurableManager::GetInstance();
    BmsCurrentsReading bms_currents_reading_1 =
        measurable_manager->ReadNanopbMeasurable<BmsCurrentsReading>(
            kEpsBmsCurrentsReading1, 1);
    BmsCurrentsReading bms_currents_reading_2 =
        measurable_manager->ReadNanopbMeasurable<BmsCurrentsReading>(
            kEpsBmsCurrentsReading2, 1);

    // BMS 1 tests
    CHECK_FALSE(bms_currents_reading_1.battery_current ==
                BmsCurrentsReading_battery_current_default);
    DOUBLES_EQUAL(0, bms_currents_reading_1.battery_current, 4.0);

    CHECK_FALSE(bms_currents_reading_1.input_current ==
                BmsCurrentsReading_input_current_default);
    DOUBLES_EQUAL(0, bms_currents_reading_1.input_current, 1.0);

    CHECK_FALSE(bms_currents_reading_1.timestamp_ms ==
                BmsCurrentsReading_timestamp_ms_default);

    // BMS 2 tests
    CHECK_FALSE(bms_currents_reading_2.battery_current ==
                BmsCurrentsReading_battery_current_default);
    DOUBLES_EQUAL(0, bms_currents_reading_2.battery_current, 4.0);

    CHECK_FALSE(bms_currents_reading_2.input_current ==
                BmsCurrentsReading_input_current_default);
    DOUBLES_EQUAL(0, bms_currents_reading_2.input_current, 1.0);

    CHECK_FALSE(bms_currents_reading_2.timestamp_ms ==
                BmsCurrentsReading_timestamp_ms_default);
}

TEST(Bms, TestBmsOperationValuesReading) {
    MeasurableManager* measurable_manager = MeasurableManager::GetInstance();
    BmsOperationValuesReading bms_operation_values_reading_1 =
        measurable_manager->ReadNanopbMeasurable<BmsOperationValuesReading>(
            kEpsBmsOperationValuesReading1, 1);
    BmsOperationValuesReading bms_operation_values_reading_2 =
        measurable_manager->ReadNanopbMeasurable<BmsOperationValuesReading>(
            kEpsBmsOperationValuesReading2, 1);

    // BMS 1 tests
    CHECK_FALSE(bms_operation_values_reading_1.recharge_threshold ==
                BmsOperationValuesReading_recharge_threshold_default);
    DOUBLES_EQUAL(0, bms_operation_values_reading_1.recharge_threshold, 3.8);

    CHECK_FALSE(bms_operation_values_reading_1.c_over_x_threshold ==
                BmsOperationValuesReading_c_over_x_threshold_default);
    CHECK(bms_operation_values_reading_1.c_over_x_threshold ==
          ((Bms::kCOverXThresholdConfigurationUBValue << 8) |
           Bms::kCOverXThresholdConfigurationLBValue));

    CHECK_FALSE(bms_operation_values_reading_1.q_count_prescale_factor ==
                BmsOperationValuesReading_q_count_prescale_factor_default);
    CHECK(bms_operation_values_reading_1.q_count_prescale_factor ==
          ((Bms::kQCountPrescaleFactorConfigurationUBValue << 8) |
           Bms::kQCountPrescaleFactorConfigurationLBValue));

    CHECK_FALSE(bms_operation_values_reading_1.timestamp_ms ==
                BmsOperationValuesReading_timestamp_ms_default);

    // BMS 2 tests
    CHECK_FALSE(bms_operation_values_reading_2.recharge_threshold ==
                BmsOperationValuesReading_recharge_threshold_default);
    DOUBLES_EQUAL(0, bms_operation_values_reading_2.recharge_threshold, 3.8);

    CHECK_FALSE(bms_operation_values_reading_2.c_over_x_threshold ==
                BmsOperationValuesReading_c_over_x_threshold_default);
    CHECK(bms_operation_values_reading_2.c_over_x_threshold ==
          ((Bms::kCOverXThresholdConfigurationUBValue << 8) |
           Bms::kCOverXThresholdConfigurationLBValue));

    CHECK_FALSE(bms_operation_values_reading_2.q_count_prescale_factor ==
                BmsOperationValuesReading_q_count_prescale_factor_default);
    CHECK(bms_operation_values_reading_2.q_count_prescale_factor ==
          ((Bms::kQCountPrescaleFactorConfigurationUBValue << 8) |
           Bms::kQCountPrescaleFactorConfigurationLBValue));

    CHECK_FALSE(bms_operation_values_reading_2.timestamp_ms ==
                BmsOperationValuesReading_timestamp_ms_default);
}

TEST(Bms, TestBmsSettingsRead) {
    MeasurableManager* measurable_manager = MeasurableManager::GetInstance();
    BmsSettingsReading bms_settings_reading_1 =
        measurable_manager->ReadNanopbMeasurable<BmsSettingsReading>(
            kEpsBmsSettingsReading1, 1);
    BmsSettingsReading bms_settings_reading_2 =
        measurable_manager->ReadNanopbMeasurable<BmsSettingsReading>(
            kEpsBmsSettingsReading2, 1);

    // BMS 1 tests

    // TODO(dingbenjamin): Add a test that checks the Jeita reigon i charge/v
    // charge is being set properly

    // Page 22 in datasheet mentions that Jeita temperature controlled charging
    // does not use VCHARGE_SETTING or ICHARGE_TARGET for setting target values

    CHECK_FALSE(bms_settings_reading_1.v_in_uvcl_setting ==
                BmsSettingsReading_v_in_uvcl_setting_default);
    CHECK(bms_settings_reading_1.v_in_uvcl_setting ==
          Bms::kVinUvclSettingConfigurationValue);

    CHECK_FALSE(bms_settings_reading_1.charger_config ==
                BmsSettingsReading_charger_config_default);
    CHECK(bms_settings_reading_1.charger_config <= 7);

    CHECK_FALSE(bms_settings_reading_1.config_bits ==
                BmsSettingsReading_config_bits_default);
    CHECK(bms_settings_reading_1.config_bits ==
          ((Bms::kConfigBitsConfigurationUBValue << 8) |
           Bms::kConfigBitsConfigurationLBValue));

    CHECK(bms_settings_reading_1.telemetry_valid);

    CHECK_FALSE(bms_settings_reading_1.timestamp_ms ==
                BmsSettingsReading_timestamp_ms_default);

    // BMS 2 tests

    // TODO(dingbenjamin): Add a test that checks the Jeita reigon i charge/v
    // charge is being set properly

    // Page 22 in datasheet mentions that Jeita temperature controlled charging
    // does not use VCHARGE_SETTING or ICHARGE_TARGET for setting target values

    CHECK_FALSE(bms_settings_reading_2.v_in_uvcl_setting ==
                BmsSettingsReading_v_in_uvcl_setting_default);
    CHECK(bms_settings_reading_2.v_in_uvcl_setting ==
          Bms::kVinUvclSettingConfigurationValue);

    CHECK_FALSE(bms_settings_reading_2.charger_config ==
                BmsSettingsReading_charger_config_default);
    CHECK(bms_settings_reading_1.charger_config <= 7);

    CHECK_FALSE(bms_settings_reading_2.config_bits ==
                BmsSettingsReading_config_bits_default);
    CHECK(bms_settings_reading_2.config_bits ==
          ((Bms::kConfigBitsConfigurationUBValue << 8) |
           Bms::kConfigBitsConfigurationLBValue));

    CHECK(bms_settings_reading_2.telemetry_valid);

    CHECK_FALSE(bms_settings_reading_2.timestamp_ms ==
                BmsSettingsReading_timestamp_ms_default);
}

TEST(Bms, TestBmsTemperatureReading) {
    MeasurableManager* measurable_manager = MeasurableManager::GetInstance();

    BmsTemperatureReading bms_temperature_reading_1 =
        measurable_manager->ReadNanopbMeasurable<BmsTemperatureReading>(
            kEpsBmsTemperatureReading1, 1);
    BmsTemperatureReading bms_temperature_reading_2 =
        measurable_manager->ReadNanopbMeasurable<BmsTemperatureReading>(
            kEpsBmsTemperatureReading2, 1);

    // BMS 1 tests
    CHECK_FALSE(bms_temperature_reading_1.die_temperature ==
                BmsTemperatureReading_die_temperature_default);
    DOUBLES_EQUAL(5, bms_temperature_reading_1.die_temperature, 55);

    CHECK_FALSE(bms_temperature_reading_1.battery_temperature ==
                BmsTemperatureReading_battery_temperature_default);
    DOUBLES_EQUAL(5, bms_temperature_reading_1.battery_temperature, 55);

    CHECK_FALSE(bms_temperature_reading_1.jeita_region ==
                BmsTemperatureReading_jeita_region_default);
    CHECK(bms_temperature_reading_1.jeita_region >= 1 &&
          bms_temperature_reading_1.jeita_region <= 7);

    CHECK_FALSE(bms_temperature_reading_1.timestamp_ms ==
                BmsTemperatureReading_timestamp_ms_default);

    // BMS 2 tests
    CHECK_FALSE(bms_temperature_reading_2.die_temperature ==
                BmsTemperatureReading_die_temperature_default);
    DOUBLES_EQUAL(5, bms_temperature_reading_2.die_temperature, 55);

    CHECK_FALSE(bms_temperature_reading_2.battery_temperature ==
                BmsTemperatureReading_battery_temperature_default);
    DOUBLES_EQUAL(5, bms_temperature_reading_2.battery_temperature, 55);

    CHECK_FALSE(bms_temperature_reading_2.jeita_region ==
                BmsTemperatureReading_jeita_region_default);
    CHECK(bms_temperature_reading_2.jeita_region >= 1 &&
          bms_temperature_reading_2.jeita_region <= 7);

    CHECK_FALSE(bms_temperature_reading_2.timestamp_ms ==
                BmsTemperatureReading_timestamp_ms_default);
}

TEST(Bms, TestBmsVoltagesReading) {
    MeasurableManager* measurable_manager = MeasurableManager::GetInstance();

    BmsVoltagesReading bms_voltages_reading_1 =
        measurable_manager->ReadNanopbMeasurable<BmsVoltagesReading>(
            kEpsBmsVoltagesReading1, 1);
    BmsVoltagesReading bms_voltages_reading_2 =
        measurable_manager->ReadNanopbMeasurable<BmsVoltagesReading>(
            kEpsBmsVoltagesReading2, 1);

    // BMS 1 tests
    CHECK_FALSE(bms_voltages_reading_1.battery_voltage ==
                BmsVoltagesReading_battery_voltage_default);
    DOUBLES_EQUAL(0, bms_voltages_reading_1.battery_voltage, 3.9);

    CHECK_FALSE(bms_voltages_reading_1.system_voltage ==
                BmsVoltagesReading_system_voltage_default);
    DOUBLES_EQUAL(0, bms_voltages_reading_1.system_voltage, 6.0);

    CHECK_FALSE(bms_voltages_reading_1.input_voltage ==
                BmsVoltagesReading_input_voltage_default);
    DOUBLES_EQUAL(0, bms_voltages_reading_1.input_voltage, 6.0);

    CHECK_FALSE(bms_voltages_reading_1.timestamp_ms ==
                BmsVoltagesReading_timestamp_ms_default);

    // BMS 2 tests
    CHECK_FALSE(bms_voltages_reading_2.battery_voltage ==
                BmsVoltagesReading_battery_voltage_default);
    DOUBLES_EQUAL(0, bms_voltages_reading_2.battery_voltage, 3.9);

    CHECK_FALSE(bms_voltages_reading_2.system_voltage ==
                BmsVoltagesReading_system_voltage_default);
    DOUBLES_EQUAL(0, bms_voltages_reading_2.system_voltage, 6.0);

    CHECK_FALSE(bms_voltages_reading_2.input_voltage ==
                BmsVoltagesReading_input_voltage_default);
    DOUBLES_EQUAL(0, bms_voltages_reading_2.input_voltage, 6.0);

    CHECK_FALSE(bms_voltages_reading_2.timestamp_ms ==
                BmsVoltagesReading_timestamp_ms_default);
}
