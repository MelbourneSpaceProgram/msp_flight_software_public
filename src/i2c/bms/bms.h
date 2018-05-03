#ifndef SRC_I2C_BMS_BMS_H
#define SRC_I2C_BMS_BMS_H

#include <external/etl/array.h>
#include <src/i2c/i2c.h>
#include <src/sensors/i2c_sensors/i2c_sensor.h>
#include <src/util/data_types.h>
#include <string>

class Bms : public I2cSensor {
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

    enum SystemStatus { kChargeEnable, kChargeDisable, kOther };
    uint16_t GetNTCRatio(byte register_location,
                                   etl::array<byte, 2>& read_buffer);

    uint16_t GetConfiguration(byte register_location,
                              etl::array<byte, 2>& read_buff);
    void SelectRegister(byte register_address);
    void ReadFromCurrentRegister(etl::array<byte, 2>& read_buffer);

    Bms::ChargeStatus GetChargeStatus(etl::array<byte, 2>& read_buffer);
    Bms::SystemStatus GetSystemStatus(etl::array<byte, 2>& read_buffer);
    bool GetTelemetryValid(etl::array<byte, 2>& read_buffer);
    double TakeI2cDieTempReading();
    double ConvertToTemperature(etl::array<byte, 2> read_buffer);
    double TakeI2cBatteryTempReading();
    double ConvertInoTemperature(etl::array<byte, 2> read_buffer);

    static const byte kUVCLRegisterValue = 0x8D;
    static const byte kVChargeRegisterValue = 0x0F;
    static const byte kIChargeRegisterValue = 0x01;
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

    static const byte kEmptybuffervalue = 0x00;
    static const byte kUVCLRegisterLocation = 0x16;
    static const byte kVChargeRegisterLocation = 0x1B;
    static const byte kIChargeRegisterLocation = 0x1A;
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
    static const byte kVsysLowLimitRegisterLocation = 0x05;
    static const byte kVsysLowLimitU = 0x08;
    static const byte kVsysLowLimitL = 0x4C;
    static const byte kVsysHighLimitRegisterLocation = 0x06;
    static const byte kVsysHighLimitU = 0x0A;
    static const byte kVsysHighLimitL = 0xAB;
    static const byte kVsysAlertEnableRegisterLocation = 0x0D;
    static const byte kVsysAlertEnable = 0xC0;
    static const byte kVsysAlertRegisterLocation = 0x36;

    // CHARGING TERMINATION
    static const byte kMaxCVTimeRegisterLocation = 0x1D;
    static const byte kMaxCVTimeRegisterValue = 0x00;
    static const byte kCXRegisterLocation = 0x29;
    static const byte kCXRegisterValue = 0x04;
    static const byte kCXThresholdRegisterLocation = 0x1C;
    static const byte kCXThresholdRegisterValue = 0x88;

    //%COULOMB Counting
    static const byte kCoulomConfigRegisterLocation = 0x14;
    static const byte kCoulomConfigRegisterValue = 0x14;
    static const byte kQCountInitialRegisterLocation = 0x13;
    static const byte kQCountInitialRegisterValue = 0x00;
    static const byte kPrescaleFactorRegisterLocation = 0x12;
    static const byte kPrescaleFactorRegisterValue = 0x03;

    static const byte kDieTempRegister = 0x3F;
    static const uint16_t kDieTempOffset = 12010;
    static const double kDieTempConversionFactor = 45.6;

    static const byte kNTCRatioRegister = 0x40;
    static const double kNTCBiasResistance = 10000.0;
    static const double kNTCBitWeight = 21845.0;

    static const double kConversionCoeffientA = 0.00084220;
    static const double kConversionCoeffientB = 0.00026265;
    static const double kConversionCoeffientC = 0.00000011875;
    static const double kKelvinToCelciusOffset = 273.15;

};

#endif  // SRC_I2C_BMS_BMS_H
