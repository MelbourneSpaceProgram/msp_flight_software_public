#include <CppUTest/TestHarness.h>
#include <external/etl/array.h>
#include <src/board/board.h>
#include <src/board/i2c/bms/bms.h>
#include <src/board/i2c/i2c.h>
#include <src/config/unit_tests.h>
#include <src/messages/TemperatureReading.pb.h>
#include <src/sensors/i2c_measurable_manager.h>

// Address value must be changed for each BMS.
static const byte test_bms_address = 0x68;

TEST_GROUP(Bms) {
    void setup() {
        if (!kBmsAvailable) {
            TEST_EXIT;
        }
    };
};

TEST(Bms, TestBmsReadingsMeasurable) {
    I2cMeasurableManager* i2c_measurable_manager =
        I2cMeasurableManager::GetInstance();
    BmsReadings bms_readings =
        i2c_measurable_manager->ReadI2cMeasurable<BmsReadings>(kEpsBmsReadings1,
                                                               1);

    // check continuous measurements are sensible
    CHECK_FALSE(bms_readings.battery_voltage ==
                BmsReadings_battery_voltage_default);
    DOUBLES_EQUAL(0, bms_readings.battery_voltage, 3.9);

    CHECK_FALSE(bms_readings.battery_current ==
                BmsReadings_battery_current_default);
    DOUBLES_EQUAL(0, bms_readings.battery_current, 4.0);

    CHECK_FALSE(bms_readings.system_voltage ==
                BmsReadings_system_voltage_default);
    DOUBLES_EQUAL(0, bms_readings.system_voltage, 6.0);

    CHECK_FALSE(bms_readings.input_voltage ==
                BmsReadings_input_current_default);
    DOUBLES_EQUAL(0, bms_readings.input_voltage, 6.0);

    CHECK_FALSE(bms_readings.input_current ==
                BmsReadings_input_current_default);
    DOUBLES_EQUAL(0, bms_readings.input_current, 1.0);

    CHECK_FALSE(bms_readings.die_temperature ==
                BmsReadings_die_temperature_default);
    DOUBLES_EQUAL(5, bms_readings.die_temperature, 55);

    CHECK_FALSE(bms_readings.battery_temperature ==
                BmsReadings_battery_temperature_default);
    DOUBLES_EQUAL(5, bms_readings.battery_temperature, 55);

    CHECK_FALSE(bms_readings.recharge_threshold ==
                BmsReadings_recharge_threshold_default);
    DOUBLES_EQUAL(0, bms_readings.recharge_threshold, 3.8);

    // check configured members have retained configuration value
    CHECK_FALSE(bms_readings.c_over_x_threshold ==
                BmsReadings_c_over_x_threshold_default);
    CHECK(bms_readings.c_over_x_threshold ==
          ((Bms::kCOverXThresholdConfigurationUBValue << 8) |
           Bms::kCOverXThresholdConfigurationLBValue));

    CHECK_FALSE(bms_readings.v_charge_setting ==
                BmsReadings_v_charge_setting_default);
    CHECK(bms_readings.v_charge_setting ==
          Bms::kVchargeSettingConfigurationValue);

    CHECK_FALSE(bms_readings.config_bits == BmsReadings_config_bits_default);
    CHECK(bms_readings.config_bits ==
          ((Bms::kConfigBitsConfigurationUBValue << 8) |
           Bms::kConfigBitsConfigurationLBValue));

    CHECK_FALSE(bms_readings.q_count == BmsReadings_q_count_default);
    CHECK(bms_readings.q_count ==
          ((Bms::kQCountRegisterConfigurationUBValue << 8) |
           Bms::kQCountRegisterConfigurationLBValue));

    CHECK_FALSE(bms_readings.q_count_prescale_factor ==
                BmsReadings_q_count_prescale_factor_default);
    CHECK(bms_readings.q_count_prescale_factor ==
          ((Bms::kQCountPrescaleFactorConfigurationUBValue << 8) |
           Bms::kQCountPrescaleFactorConfigurationLBValue));

    CHECK_FALSE(bms_readings.v_in_uvcl_setting ==
                BmsReadings_v_in_uvcl_setting_default);
    CHECK(bms_readings.v_in_uvcl_setting ==
          Bms::kVinUvclSettingConfigurationValue);

    // check read-only members correspond to possible register values
    CHECK_FALSE(bms_readings.jeita_region == BmsReadings_jeita_region_default);
    CHECK(bms_readings.jeita_region >= 1 && bms_readings.jeita_region <= 7);

    CHECK_FALSE(bms_readings.system_status ==
                BmsReadings_system_status_default);
    CHECK(bms_readings.system_status == 0 || bms_readings.system_status == 1);

    CHECK_FALSE(bms_readings.charger_state ==
                BmsReadings_charger_state_default);
    CHECK(bms_readings.charger_state == Bms::kBigShortFault ||
          bms_readings.charger_state == Bms::kBatMissingFault ||
          bms_readings.charger_state == Bms::kMaxChargeTimeFault ||
          bms_readings.charger_state == Bms::kCOverXTerm ||
          bms_readings.charger_state == Bms::kTimerTerm ||
          bms_readings.charger_state == Bms::KNtcPause ||
          bms_readings.charger_state == Bms::kCcCvCharge ||
          bms_readings.charger_state == Bms::kPrecharge ||
          bms_readings.charger_state == Bms::kChargerSuspended ||
          bms_readings.charger_state == Bms::kAbsorbCharge ||
          bms_readings.charger_state == Bms::kEqualizeCharge ||
          bms_readings.charger_state == Bms::kInvalidChargerState);

    CHECK_FALSE(bms_readings.charge_status ==
                BmsReadings_charge_status_default);
    CHECK(bms_readings.charge_status == Bms::kConstantVoltage ||
          bms_readings.charge_status == Bms::kConstantCurrent ||
          bms_readings.charge_status == Bms::kIinLimitActive ||
          bms_readings.charge_status == Bms::kVinLimitActive ||
          bms_readings.charge_status == Bms::kNotCharging ||
          bms_readings.charge_status == Bms::kError);

    CHECK_FALSE(bms_readings.charger_config ==
                BmsReadings_charger_config_default);
    CHECK(bms_readings.charger_config <= 7);

    CHECK_FALSE(bms_readings.v_charge_dac == BmsReadings_v_charge_dac_default);
    CHECK(bms_readings.v_charge_dac <= 63);

    CHECK_FALSE(bms_readings.i_charge_dac == BmsReadings_i_charge_dac_default);
    CHECK(bms_readings.i_charge_dac <= 31);

    // check i2c is working
    CHECK(bms_readings.telemetry_valid);
}

TEST(Bms, TestBms) {
    I2c bus_d(I2C_BUS_D);
    Bms bms(&bus_d, test_bms_address);

    CHECK_EQUAL(Bms::kVinUvclSettingConfigurationValue,
                bms.GetVinUvclSetting());
    CHECK_EQUAL(Bms::kVchargeSettingConfigurationValue,
                bms.GetVchargeSetting());
    CHECK_EQUAL(Bms::kIChargeTargetConfigurationValue, bms.GetIchargeTarget());
    CHECK_EQUAL((Bms::kRechargeThresholdConfigurationUBValue << 8 |
                 Bms::kRechargeThresholdConfigurationLBValue),
                bms.GetRechargeThreshold());

    CHECK(Bms::kError != bms.GetChargeStatus());
}

TEST(Bms, TestBmsDieTemperatureRead) {
    I2cMeasurableManager* i2c_measurable_manager =
        I2cMeasurableManager::GetInstance();
    double temp = i2c_measurable_manager
                      ->ReadI2cMeasurable<TemperatureReading>(kEpsBmsDieT1, 1)
                      .temp;
    DOUBLES_EQUAL(25, temp, 50);
}

TEST(Bms, TestBmsBatteryTemperatureRead) {
    I2cMeasurableManager* i2c_measurable_manager =
        I2cMeasurableManager::GetInstance();
    double temp = i2c_measurable_manager
                      ->ReadI2cMeasurable<TemperatureReading>(kEpsBmsBatT1, 1)
                      .temp;
    DOUBLES_EQUAL(25, temp, 50);
}

TEST(Bms, TestJeitaRegion) {
    I2c bus_d(I2C_BUS_D);
    Bms bms(&bus_d, test_bms_address);

    uint16_t jeita_region = 3;
    double v_charge = 3.6;
    double i_charge = 1.0;

    CHECK_EQUAL(jeita_region, bms.GetJeitaRegion());
    DOUBLES_EQUAL(v_charge, bms.GetVchargeDac(), 0.5);
    DOUBLES_EQUAL(i_charge, bms.GetIchargeDac(), 0.5);
}
