#ifndef SRC_BOARD_I2C_BMS_BMS_H_
#define SRC_BOARD_I2C_BMS_BMS_H_

#include <external/etl/array.h>
#include <src/messages/BmsChargingInfoReading.pb.h>
#include <src/messages/BmsCurrentsReading.pb.h>
#include <src/messages/BmsOperationValuesReading.pb.h>
#include <src/messages/BmsSettingsReading.pb.h>
#include <src/messages/BmsTemperatureReading.pb.h>
#include <src/messages/BmsVoltagesReading.pb.h>
#include <src/sensors/i2c_sensors/i2c_device.h>
#include <src/util/data_types.h>
#include <string>

class I2c;

class Bms : public I2cDevice {
    friend class TEST_Bms_TestBmsChargingInfoRead_Test;
    friend class TEST_Bms_TestBmsCurrentsReading_Test;
    friend class TEST_Bms_TestBmsOperationValuesReading_Test;
    friend class TEST_Bms_TestBmsSettingsRead_Test;
    friend class TEST_Bms_TestBmsTemperatureReading_Test;
    friend class TEST_Bms_TestBmsVoltagesReading_Test;

   public:
    Bms(const I2c* bus, int address, const I2cMultiplexer* multiplexer = NULL,
        I2cMultiplexer::MuxChannel channel = I2cMultiplexer::kMuxNoChannel);
    bool SetConfiguration();
    bool WriteToRegister(byte register_location, byte lower_byte,
                         byte upper_byte);

    bool SelectRegister(byte register_address);
    bool ReadFromCurrentRegister(etl::array<byte, 2>& read_buffer);

    /* get methods */
    BmsSettingsReading GetBmsSettingsReading();
    BmsChargingInfoReading GetBmsChargingInfoReading();
    BmsOperationValuesReading GetBmsOperationValuesReading();
    BmsTemperatureReading GetBmsTemperatureReading();
    BmsVoltagesReading GetBmsVoltagesReading();
    BmsCurrentsReading GetBmsCurrentsReading();
    double GetBatteryVoltage();
    double GetBatteryCurrent();
    double GetSystemVoltage();
    double GetInputVoltage();
    double GetInputCurrent();
    double GetDieTemp();
    double GetBatteryTemp();
    uint16_t GetJeitaRegion();
    BmsChargingInfoReading_SystemStatus GetSystemStatus();
    BmsChargingInfoReading_ChargerState GetChargerState();
    BmsChargingInfoReading_ChargeStatus GetChargeStatus();
    double GetRechargeThreshold();
    uint16_t GetChargerConfig();
    uint16_t GetCOverXThreshold();
    uint16_t GetVchargeSetting();
    uint16_t GetIchargeTarget();
    uint16_t GetConfigBits();
    uint16_t GetQCount();
    uint16_t GetQCountPrescaleFactor();
    uint16_t GetVinUvclSetting();
    bool GetTelemetryValid();

    bool GetFirstChargeComplete();
    int32_t GetQCountDelta();

    /* Coulomb counting constants */
    static constexpr uint16_t kQCountFullCharge = 60000;
    static constexpr uint16_t kQCountInitial =
        12000;  // 20% of kQCountFullCharge
    static constexpr uint16_t kQCountPrescaleFactor = 28;

   private:
    /* conversion methods */
    static double ConvertToBatteryVoltage(etl::array<byte, 2>& read_buffer);
    static double ConvertToBatteryCurrent(etl::array<byte, 2>& read_buffer);
    static double ConvertToSystemVoltage(etl::array<byte, 2>& read_buffer);
    static double ConvertToInputVoltage(etl::array<byte, 2>& read_buffer);
    static double ConvertToInputCurrent(etl::array<byte, 2>& read_buffer);
    static double ConvertToDieTemperature(etl::array<byte, 2>& read_buffer);
    static double ConvertToBatteryTemperature(etl::array<byte, 2>& read_buffer);
    static uint16_t ConvertToJeitaRegion(etl::array<byte, 2>& read_buffer);
    static BmsChargingInfoReading_SystemStatus ConvertToSystemStatus(
        etl::array<byte, 2>& read_buffer);
    static BmsChargingInfoReading_ChargerState ConvertToChargerState(
        etl::array<byte, 2>& read_buffer);
    static BmsChargingInfoReading_ChargeStatus ConvertToChargeStatus(
        etl::array<byte, 2>& read_buffer);
    static double ConvertToRechargeThreshold(etl::array<byte, 2>& read_buffer);
    static uint16_t ConvertToChargerConfig(etl::array<byte, 2>& read_buffer);
    static uint16_t ConvertToCOverXThreshold(etl::array<byte, 2>& read_buffer);
    static uint16_t ConvertToVchargeSetting(etl::array<byte, 2>& read_buffer);
    static uint16_t ConvertToIchargeTarget(etl::array<byte, 2>& read_buffer);
    static uint16_t ConvertToConfigBits(etl::array<byte, 2>& read_buffer);
    static uint16_t ConvertToQCount(etl::array<byte, 2>& read_buffer);
    static uint16_t ConvertToQCountPrescaleFactor(
        etl::array<byte, 2>& read_buffer);
    static uint16_t ConvertToVinUvclSetting(etl::array<byte, 2>& read_buffer);
    static bool ConvertToTelemetryValid(etl::array<byte, 2>& read_buffer);

    template <class T>
    T GetFromRegisterAndConvert(byte register_location,
                                T(conversion_func)(etl::array<byte, 2>&),
                                T failed_reading_value) {
        etl::array<byte, 2> read_buffer;
        if (SelectRegister(register_location) &&
            ReadFromCurrentRegister(read_buffer)) {
            return conversion_func(read_buffer);
        }
        return failed_reading_value;
    }

    bool first_charge_complete;

    /* register locations */
    static constexpr byte kQCountPrescaleFactorRegisterLocation = 0x12;
    static constexpr byte kQCountRegisterLocation = 0x13;
    static constexpr byte kConfigBitsRegisterLocation = 0x14;
    static constexpr byte kVinUvclSettingRegisterLocation = 0x16;
    static constexpr byte kIchargeTargetRegisterLocation = 0x1A;
    static constexpr byte kVchargeSettingRegisterLocation = 0x1B;
    static constexpr byte kCOverXThresholdRegisterLocation = 0x1C;
    static constexpr byte kMaxCvTimeRegisterLocation = 0x1D;
    static constexpr byte kJeitaT1RegisterLocation = 0x1F;
    static constexpr byte kVchargeJeita5to6RegisterLocation = 0x25;
    static constexpr byte kVchargeJeita2to4RegisterLocation = 0x26;
    static constexpr byte kIchargeJeita5to6RegisterLocation = 0x27;
    static constexpr byte kIchargeJeita2to4RegisterLocation = 0x28;
    static constexpr byte kChargerConfigBitsRegisterLocation = 0x29;
    static constexpr byte kRechargeThresholdRegisterLocation = 0x2E;
    static constexpr byte kChargeStatusRegisterLocation = 0x35;
    static constexpr byte kSystemStatusRegisterLocation = 0x39;
    static constexpr byte kVbatRegisterLocation = 0x3A;
    static constexpr byte kVinRegisterLocation = 0x3B;
    static constexpr byte kVsysRegisterLocation = 0x3C;
    static constexpr byte kIbatRegisterLocation = 0x3D;
    static constexpr byte kIinRegisterLocation = 0x3E;
    static constexpr byte kDieTempRegisterLocation = 0x3F;
    static constexpr byte kNtcRatioRegisterLocation = 0x40;
    static constexpr byte kJeitaRegionRegisterLocation = 0x42;
    static constexpr byte kChemCellsRegisterLocation = 0x43;
    static constexpr byte kMeasSysValidRegisterLocation = 0x4A;

    /* bit masks */
    static const uint16_t kTelemetryValidBitMask = 0x0001;
    static const uint16_t kConstantVoltageBitMask = 0x0001;
    static const uint16_t kConstantCurrentBitMask = 0x0002;
    static const uint16_t kIinLimitActiveBitMask = 0x0004;
    static const uint16_t kVinLimitActiveBitMask = 0x0008;
    static const uint16_t kChargeStatusBitMask = 0x000F;
    static const uint16_t kChargeStatusNotCharging = 0x0000;
    static const uint16_t kChargeEnableBitMask = 0x2000;
    static const uint16_t kJeitaRegionBitMask = 0x0007;
    static const uint16_t kChargerConfigBitMask = 0x0007;
    static const uint16_t kVchargeSettingBitMask = 0x001F;
    static const uint16_t kIchargeTargetBitMask = 0x001F;
    static const uint16_t kUpperByteBitMask = 0xFF00;
    static const uint16_t kLowerByteBitMask = 0x00FF;

    /* initial configuration values */
    static const byte kChargerConfigBitsConfigurationValue =
        0x05;  // assert en_jeita and en_c_over_x_term
    static const byte kCOverXThresholdConfigurationLBValue = 0x88;
    static const byte kCOverXThresholdConfigurationUBValue = 0x00;
    static const byte kJeitaT1ConfigurationLBValue = 0x07;
    static const byte kJeitaT1ConfigurationUBValue = 0x4B;
    static const byte kVchargeJeita5to6ConfigurationLBValue = 0x73;
    static const byte kVchargeJeita5to6ConfigurationUBValue = 0x02;
    static const byte kVchargeJeita2to4ConfigurationLBValue = 0x73;
    static const byte kVchargeJeita2to4ConfigurationUBValue = 0x4E;
    static const byte kMaxCvTimeConfigurationValue = 0x00;
    static const byte kQCountPrescaleFactorConfigurationLBValue = 0x03;
    static const byte kQCountPrescaleFactorConfigurationUBValue = 0x00;
    static const byte kQCountRegisterConfigurationLBValue =
        (Bms::kQCountInitial & kLowerByteBitMask);
    static const byte kQCountRegisterConfigurationUBValue =
        (Bms::kQCountInitial & kUpperByteBitMask) >> 8;
    static const byte kConfigBitsConfigurationLBValue =
        0x04;  // enable coloumb counter
    static const byte kConfigBitsConfigurationUBValue = 0x00;
    static const byte kEmptyBufferValue = 0x00;
    static const byte kRechargeThresholdConfigurationLBValue = 0x0C;
    static const byte kRechargeThresholdConfigurationUBValue = 0x43;

    // Direct Energy Transfer mode
    static const byte kIchargeJeita5to6ConfigurationLBValue = 0x63;
    static const byte kIchargeJeita5to6ConfigurationUBValue = 0x00;
    static const byte kIchargeJeita2to4ConfigurationLBValue = 0x63;
    static const byte kIchargeJeita2to4ConfigurationUBValue = 0x0C;
    static const byte kVinUvclSettingConfigurationValue = 0x00;

    // Value for registers with values determined by JEITA region
    static const byte kVchargeSettingAllJeitaRegionsValue =
        0x13;  // 0x13 = 0b10011 is repeated in a 5-bit unit to produce the
               // kVchargeJeita2to4Configuration and
               // kVchargeJeita5to6Configuration values, which set the
               // v_charge_setting register to 0x13 in every JEITA region
    static const byte kIChargeTargetAllJeitaRegionsValue =
        0x01;  // 0x01 = 0b00001 us repeated in a 5-bit unit to produce the
               // kIchargeJeita2to4Configuration and
               // kIchargeJeita5to6Configuration values, which set the
               // i_charge_setting register to 0x13 in every JEITA region

    /* conversion constants */
    static constexpr double kVchargeDivisionFactor = 80.0;
    static constexpr double kVchargeAdditionFactor = 3.4125;
    static constexpr double kIchargeAdditionFactor = 1.0;
    static constexpr double kIchargeMultiplicationFactor = 0.125;
    static constexpr uint16_t kDieTempOffset = 12010;
    static constexpr double kDieTempConversionFactor = 45.6;
    static constexpr double kNtcBiasResistance = 10000.0;
    static constexpr double kNtcBitWeight = 21845.0;
    static constexpr double kConversionCoefficientA = 0.00084220;
    static constexpr double kConversionCoefficientB = 0.00026265;
    static constexpr double kConversionCoefficientC = 0.00000011875;
    static constexpr double kKelvinToCelciusOffset = 273.15;

    // miscellaneous constants
    static constexpr byte kChargerStateRegisterLocation = 0x34;
    static constexpr byte kChargerConfigRegisterLocation = 0x29;
    static constexpr byte kConfigurationBitsRegisterLocation = 0x14;

    static constexpr double kRSnsbResistance = 0.033;
    static constexpr double kRSnsiResistance = 0.002;
    static constexpr double kLithiumBatteryVoltageConversionFactor =
        0.000192264;
    static constexpr double kBatteryCurrentConversionFactor =
        0.00000146487 / kRSnsbResistance;
    static constexpr double kSystemVoltageConversionFactor = 0.001648;
    static constexpr double kInputVoltageConversionFactor = 0.001648;
    static constexpr double kInputCurrentConversionFactor =
        0.00000146487 / kRSnsbResistance;
    static constexpr double kRechargeThresholdConversionFactor = 0.000192264;
};

#endif  // SRC_BOARD_I2C_BMS_BMS_H_
