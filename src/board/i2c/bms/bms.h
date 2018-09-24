#ifndef SRC_BOARD_I2C_BMS_BMS_H_
#define SRC_BOARD_I2C_BMS_BMS_H_

#include <external/etl/array.h>
#include <src/messages/BmsReadings.pb.h>
#include <src/sensors/i2c_sensors/i2c_device.h>
#include <src/util/data_types.h>
#include <string>

class I2c;

class Bms : public I2cDevice {
   public:
    Bms(const I2c* bus, int address, const I2cMultiplexer* multiplexer = NULL,
        I2cMultiplexer::MuxChannel channel = I2cMultiplexer::kMuxNoChannel);
    void SetConfiguration();
    bool WriteToRegister(byte register_location, byte lower_byte,
                         byte upper_byte);

    bool SelectRegister(byte register_address);
    bool ReadFromCurrentRegister(etl::array<byte, 2>& read_buffer);

    BmsReadings GetBmsReadings();
    double GetBatteryVoltage();
    double GetBatteryCurrent();
    double GetSystemVoltage();
    double GetInputVoltage();
    double GetInputCurrent();
    double GetDieTemp();
    double GetBatteryTemp();
    uint16_t GetJeitaRegion();
    BmsReadings_SystemStatus GetSystemStatus();
    BmsReadings_ChargerState GetChargerState();
    BmsReadings_ChargeStatus GetChargeStatus();
    double GetRechargeThreshold();
    uint16_t GetChargerConfig();
    uint16_t GetCOverXThreshold();
    uint16_t GetVchargeSetting();
    uint16_t GetIchargeTarget();
    uint16_t GetConfigBits();
    uint16_t GetQCount();
    uint16_t GetQCountPrescaleFactor();
    uint16_t GetVinUvclSetting();
    double GetVchargeDac();
    double GetIchargeDac();
    bool GetTelemetryValid();

    /* initial configuration values */
    static const byte kChargerConfigBitsConfigurationValue =
        0x05;  // assert en_jeita and en_c_over_x_term
    static const byte kCOverXThresholdConfigurationLBValue = 0x88;
    static const byte kCOverXThresholdConfigurationUBValue = 0x00;
    static const byte kJeitaT1ConfigurationLBValue = 0x07;
    static const byte kJeitaT1ConfigurationUBValue = 0x4B;
    static const byte kVchargeJeita5to6ConfigurationLBValue = 0x02;
    static const byte kVchargeJeita5to6ConfigurationUBValue = 0x73;
    static const byte kVchargeJeita2to4ConfigurationLBValue = 0x4E;
    static const byte kVchargeJeita2to4ConfigurationUBValue = 0x73;
    static const byte kIchargeJeita5to6ConfigurationLBValue = 0xFF;
    static const byte kIchargeJeita5to6ConfigurationUBValue = 0x03;
    static const byte kIchargeJeita2to4ConfigurationLBValue = 0xFF;
    static const byte kIchargeJeita2to4ConfigurationUBValue = 0x7F;
    static const byte kMaxCvTimeConfigurationValue = 0x00;
    static const byte kQCountPrescaleFactorConfigurationLBValue = 0x03;
    static const byte kQCountPrescaleFactorConfigurationUBValue = 0x00;
    static const byte kQCountRegisterConfigurationLBValue = 0x00;
    static const byte kQCountRegisterConfigurationUBValue = 0x00;
    static const byte kConfigBitsConfigurationLBValue = 0x04;
    static const byte kConfigBitsConfigurationUBValue = 0x00;
    static const byte kVchargeSettingConfigurationValue = 0x13;
    static const byte kIChargeTargetConfigurationValue = 0x07;
    static const byte kEmptyBufferValue = 0x00;
    static const byte kRechargeThresholdConfigurationLBValue = 0x0C;
    static const byte kRechargeThresholdConfigurationUBValue = 0x43;
    static const byte kVinUvclSettingConfigurationValue = 0xFF;

   private:
    // conversion methods
    static double ConvertToBatteryVoltage(etl::array<byte, 2>& read_buffer);
    static double ConvertToBatteryCurrent(etl::array<byte, 2>& read_buffer);
    static double ConvertToSystemVoltage(etl::array<byte, 2>& read_buffer);
    static double ConvertToInputVoltage(etl::array<byte, 2>& read_buffer);
    static double ConvertToInputCurrent(etl::array<byte, 2>& read_buffer);
    static double ConvertToDieTemperature(etl::array<byte, 2>& read_buffer);
    static double ConvertToBatteryTemperature(etl::array<byte, 2>& read_buffer);
    static uint16_t ConvertToJeitaRegion(etl::array<byte, 2>& read_buffer);
    static BmsReadings_SystemStatus ConvertToSystemStatus(
        etl::array<byte, 2>& read_buffer);
    static BmsReadings_ChargerState ConvertToChargerState(
        etl::array<byte, 2>& read_buffer);
    static BmsReadings_ChargeStatus ConvertToChargeStatus(
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
    static double ConvertToVchargeDac(etl::array<byte, 2>& read_buffer);
    static double ConvertToIchargeDac(etl::array<byte, 2>& read_buffer);
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

    /* register locations */
    static const byte kQCountPrescaleFactorRegisterLocation = 0x12;
    static const byte kQCountRegisterLocation = 0x13;
    static const byte kConfigBitsRegisterLocation = 0x14;
    static const byte kVinUvclSettingRegisterLocation = 0x16;
    static const byte kIchargeTargetRegisterLocation = 0x1A;
    static const byte kVchargeSettingRegisterLocation = 0x1B;
    static const byte kCOverXThresholdRegisterLocation = 0x1C;
    static const byte kMaxCvTimeRegisterLocation = 0x1D;
    static const byte kJeitaT1RegisterLocation = 0x1F;
    static const byte kVchargeJeita5to6RegisterLocation = 0x25;
    static const byte kVchargeJeita2to4RegisterLocation = 0x26;
    static const byte kIchargeJeita5to6RegisterLocation = 0x27;
    static const byte kIchargeJeita2to4RegisterLocation = 0x28;
    static const byte kChargerConfigBitsRegisterLocation = 0x29;
    static const byte kRechargeThresholdRegisterLocation = 0x2E;
    static const byte kChargeStatusRegisterLocation = 0x35;
    static const byte kSystemStatusRegisterLocation = 0x39;
    static const byte kVbatRegisterLocation = 0x3A;
    static const byte kVinRegisterLocation = 0x3B;
    static const byte kVsysRegisterLocation = 0x3C;
    static const byte kIbatRegisterLocation = 0x3D;
    static const byte kIinRegisterLocation = 0x3E;
    static const byte kDieTempRegisterLocation = 0x3F;
    static const byte kNtcRatioRegisterLocation = 0x40;
    static const byte kJeitaRegionRegisterLocation = 0x42;
    static const byte kChemCellsRegisterLocation = 0x43;
    static const byte kIchargeDacRegisterLocation = 0x44;
    static const byte kVchargeDacRegisterLocation = 0x45;
    static const byte kMeasSysValidRegisterLocation = 0x4A;

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
    static const uint16_t kIchargeTargetBitMask = 0x000F;

    /* conversion constants */
    static const double kVchargeDivisionFactor = 80.0;
    static const double kVchargeAdditionFactor = 3.4125;
    static const double kIchargeAdditionFactor = 1.0;
    static const double kIchargeMultiplicationFactor = 0.125;
    static const uint16_t kDieTempOffset = 12010;
    static const double kDieTempConversionFactor = 45.6;
    static const double kNtcBiasResistance = 10000.0;
    static const double kNtcBitWeight = 21845.0;
    static const double kConversionCoefficientA = 0.00084220;
    static const double kConversionCoefficientB = 0.00026265;
    static const double kConversionCoefficientC = 0.00000011875;
    static const double kKelvinToCelciusOffset = 273.15;

    // HUGO'S CONSTANTS (should probably be renamed by someone who knows BMS
    // terminology better!)
    static const byte kChargerStateRegisterLocation = 0x34;
    static const byte kChargerConfigRegisterLocation = 0x29;
    static const byte kConfigurationBitsRegisterLocation = 0x14;

    static constexpr double kRSnsbResistance = 0.033;
    static const double kRSnsiResistance = 0.002;
    static const double kLithiumBatteryVoltageConversionFactor = 0.000192264;
    static const double kBatteryCurrentConversionFactor =
        0.00000146487 / kRSnsbResistance;
    static const double kSystemVoltageConversionFactor = 0.001648;
    static const double kInputVoltageConversionFactor = 0.001648;
    static const double kInputCurrentConversionFactor =
        0.00000146487 / kRSnsbResistance;
    static const double kRechargeThresholdConversionFactor = 0.000192264;
};

#endif  // SRC_BOARD_I2C_BMS_BMS_H_
