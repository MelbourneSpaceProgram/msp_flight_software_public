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

    enum ChargeStatus {
        kConstantVoltage,
        kConstantCurrent,
        kIinLimitActive,
        kVinLimitActive,
        kNotCharging,
        kError
    };

    enum ChargerStates {
        kBigShortFault = 0x0001,
        kBatMissingFault = 0x0002,
        kMaxChargeTimeFault = 0x0004,
        kCOverXTerm = 0x0008,
        kTimerTerm = 0x0010,
        KNtcPause = 0x0020,
        kCcCvCharge = 0x0040,
        kPrecharge = 0x0080,
        kChargerSuspended = 0x0100,
        kAbsorbCharge = 0x0200,
        kEqualizeCharge = 0x0400
    };

    enum SystemStatus { kChargeEnable, kChargeDisable, kOther };

    // TODO(hugorilla): implement this (if needed)
    uint16_t GetNTCRatio(byte register_location,
                         etl::array<byte, 2>& read_buffer);

    uint16_t GetConfiguration(byte register_location,
                              etl::array<byte, 2>& read_buff);
    void SelectRegister(byte register_address);
    void ReadFromCurrentRegister(etl::array<byte, 2>& read_buffer);

    BmsReadings GetBmsReadings();

    double GetBatteryVoltage();
    double ConvertToBatteryVoltage(etl::array<byte, 2>& read_buffer);
    double GetBatteryCurrent();
    double ConvertToBatteryCurrent(etl::array<byte, 2>& read_buffer);
    double GetSystemVoltage();
    double ConvertToSystemVoltage(etl::array<byte, 2>& read_buffer);
    double GetInputVoltage();
    double ConvertToInputVoltage(etl::array<byte, 2>& read_buffer);
    double GetInputCurrent();
    double ConvertToInputCurrent(etl::array<byte, 2>& read_buffer);
    double GetDieTemp();
    double ConvertToDieTemperature(etl::array<byte, 2>& read_buffer);
    double GetBatteryTemp();
    double ConvertToBatteryTemperature(etl::array<byte, 2>& read_buffer);
    uint16_t GetJeitaRegion();
    uint16_t ConvertToJeitaRegion(etl::array<byte, 2>& read_buffer);
    Bms::SystemStatus GetSystemStatus(etl::array<byte, 2>& read_buffer);
    Bms::ChargerStates GetChargerState();
    Bms::ChargerStates ConvertToChargerState(etl::array<byte, 2>& read_buffer);
    Bms::ChargeStatus GetChargeStatus(etl::array<byte, 2>& read_buffer);
    double GetRechargeThreshold();
    double ConvertToRechargeThreshold(etl::array<byte, 2>& read_buffer);
    uint16_t GetChargerConfig();
    uint16_t ConvertToChargerConfig(etl::array<byte, 2>& read_buffer);
    uint16_t GetCXThreshold();
    uint16_t ConvertToCXThreshold(etl::array<byte, 2>& read_buffer);
    uint16_t GetChargeVoltageSetting();
    uint16_t ConvertToChargeVoltageSetting(etl::array<byte, 2>& read_buffer);
    uint16_t GetChargeCurrentTarget();
    uint16_t ConvertToChargeCurrentTarget(etl::array<byte, 2>& read_buffer);
    uint16_t GetConfiguration();
    uint16_t GetQCount();
    uint16_t ConvertToQCount(etl::array<byte, 2>& read_buffer);
    uint16_t GetQCountPrescaleFactor();
    uint16_t ConvertToQCountPrescaleFactor(etl::array<byte, 2>& read_buffer);
    bool GetTelemetryValid(etl::array<byte, 2>& read_buffer);

    uint16_t GetJeitaRegionVCharge(etl::array<byte, 2>& read_buffer);
    double GetVChargeDEC(etl::array<byte, 2>& read_buffer);
    double GetIChargeDEC(etl::array<byte, 2>& read_buffer);

    static const byte kUVCLRegisterValue = 0x8D;
    static const byte kVChargeSettingRegisterValue = 0x0F;
    static const byte kIChargeRegisterValue = 0x07;

    // TODO(hugorilla): Check these values against the data sheet
    static const byte kReChargeThresholdLRegisterValue = 0x0C;
    static const byte kReChargeThresholdURegisterValue = 0x43;

   private:
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
    static const uint16_t kChargeVoltageSettingBitMask = 0x001F;
    static const uint16_t kChargeCurrentTargetBitMask = 0x000F;

    static const byte kEmptybuffervalue = 0x00;
    static const byte kUVCLRegisterLocation = 0x16;
    static const byte kVChargeSettingRegisterLocation = 0x1B;
    static const byte kIChargeTargetRegisterLocation = 0x1A;
    static const byte kVChargeDACRegisterLocation = 0x45;
    static const byte kIChargeDACRegisterLocation = 0x44;
    static const byte kReChargeThresholdRegisterLocation = 0x2E;
    static const byte kVinRegisterLocation = 0x3B;
    static const byte kIinRegisterLocation = 0x3E;
    static const byte kVsysRegisterLocation = 0x3C;
    static const byte kVbatRegisterLocation = 0x3A;
    static const byte kIbatRegisterLocation = 0x3D;
    static const byte kChargeStatusRegisterLocation = 0x35;
    static const byte kChemandCellsRegisterLocation = 0x43;
    static const byte kSystemStatusRegisterLocation = 0x39;
    static const byte kTelemetryValidRegisterLocation = 0x4A;

    // CHARGING TERMINATION
    static const byte kMaxCVTimeRegisterLocation = 0x1D;
    static const byte kMaxCVTimeRegisterValue = 0x00;

    // check this register value
    static const byte kCXJeitaEnableRegisterLocation = 0x29;

    static const byte kCXJeitaEnableRegisterValue = 0x05;
    static const byte kCXThresholdRegisterLocation = 0x1C;
    static const byte kCXThresholdRegisterValue = 0x88;
    static const byte kJeitaRegionRegisterLocation = 0x42;
    static const byte kJeitaT1RegisterLocation = 0x1F;
    static const byte kJeitaT1ConfiqurationUBValue = 0x4B;
    static const byte kJeitaT1ConfiqurationLBValue = 0x07;
    static const byte kVChargeJeita5to6RegisterLocation = 0x25;
    static const byte kVChargeJeita5to6ConfigurationLBValue = 0x02;
    static const byte kVChargeJeita5to6ConfigurationUBValue = 0x73;
    static const byte kVChargeJeita2to4RegisterLocation = 0x26;
    static const byte kVChargeJeita2to4ConfigurationLBValue = 0x4E;
    static const byte kVChargeJeita2to4ConfigurationUBValue = 0x73;
    static const byte kIChargeJeita5to6RegisterLocation = 0x27;
    static const byte kIChargeJeita5to6ConfigurationLBValue = 0xFF;
    static const byte kIChargeJeita5to6ConfigurationUBValue = 0x03;
    static const byte kIChargeJeita2to4RegisterLocation = 0x28;
    static const byte kIChargeJeita2to4ConfigurationLBValue = 0xFF;
    static const byte kIChargeJeita2to4ConfigurationUBValue = 0x7F;
    static const double kVchargeDivisionFactor = 80.0;
    static const double kVchargeAdditionFactor = 3.4125;
    static const double kIchargeAdditionFactor = 1.0;
    static const double kIchargeMultiplicationFactor = 0.125;

    //%COULOMB Counting
    // TODO(hugorilla): update kCoulumbConfigRegisterLocation - it's just the
    // config registser, so this could get confusing
    static const byte kCoulombConfigRegisterLocation = 0x14;
    static const byte kCoulombConfigRegisterInitialValue = 0x14;
    static const byte kQCountRegisterLocation = 0x13;
    static const byte kQCountRegisterInitialValue = 0x00;
    static const byte kQCountPrescaleFactorRegisterLocation = 0x12;
    static const byte kQCountPrescaleFactorRegisterInitialValue = 0x03;

    static const byte kDieTempRegister = 0x3F;
    static const uint16_t kDieTempOffset = 12010;
    static const double kDieTempConversionFactor = 45.6;

    static const byte kNTCRatioRegister = 0x40;
    static const double kNTCBiasResistance = 10000.0;
    static const double kNTCBitWeight = 21845.0;

    static const double kConversionCoefficientA = 0.00084220;
    static const double kConversionCoefficientB = 0.00026265;
    static const double kConversionCoefficientC = 0.00000011875;
    static const double kKelvinToCelciusOffset = 273.15;

    // HUGO'S CONSTANTS (should probably be renamed by someone who knows BMS
    // terminology better!)
    static const byte kChargerStateRegisterLocation = 0x34;
    static const byte kChargerConfigRegisterLocation = 0x29;
    static const byte kConfigurationBitsRegisterLocation = 0x14;

    static const double kRSnsbResistance = 0.033;
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
