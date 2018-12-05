#ifndef SRC_DATABASE_FILE_OFFSETS_H_
#define SRC_DATABASE_FILE_OFFSETS_H_

#include <src/config/satellite.h>
#include <src/messages/AccelerometerReading.pb.h>
#include <src/messages/BDotEstimate.pb.h>
#include <src/messages/BmsChargingInfoReading.pb.h>
#include <src/messages/BmsCurrentsReading.pb.h>
#include <src/messages/BmsOperationValuesReading.pb.h>
#include <src/messages/BmsSettingsReading.pb.h>
#include <src/messages/BmsTemperatureReading.pb.h>
#include <src/messages/BmsVoltagesReading.pb.h>
#include <src/messages/CurrentReading.pb.h>
#include <src/messages/GyroscopeReading.pb.h>
#include <src/messages/MagnetometerReading.pb.h>
#include <src/messages/TemperatureReading.pb.h>
#include <src/messages/VoltageReading.pb.h>
#include <src/sensors/measurable_id.h>

constexpr int kMeasurableReadingSize[kMeasurableIdEnd] = {
    VoltageReading_size,             //    kComInV1
    VoltageReading_size,             //    kComOutV1
    VoltageReading_size,             //    kComInV2
    VoltageReading_size,             //    kComOutV2
    CurrentReading_size,             //    kComInI1
    CurrentReading_size,             //    kComOutI1
    CurrentReading_size,             //    kComInI2
    CurrentReading_size,             //    kComOutI2
    TemperatureReading_size,         //    kComT1
    TemperatureReading_size,         //    kComT2
                                     //    kComRxBytes
                                     //    kComTxBytes
                                     //    kComAntennaFlags
                                     //    kComLithiumOps
                                     //    kComLithiumRssi
                                     //    kComLithiumT
                                     //    kComLithiumTime
    TemperatureReading_size,         //    kEpsT1
    TemperatureReading_size,         //    kEpsT2
    TemperatureReading_size,         //    kEpsBmsDieT1
    TemperatureReading_size,         //    kEpsBmsDieT2
    TemperatureReading_size,         //    kEpsBmsBatT1
    TemperatureReading_size,         //    kEpsBmsBatT2
    VoltageReading_size,             //    kEpsAdcBatV1
    VoltageReading_size,             //    kEpsBoostOutV1
    CurrentReading_size,             //    kEpsBoostInI1
    CurrentReading_size,             //    kEpsLoadI1
    VoltageReading_size,             //    kEpsAdcBatV2
    VoltageReading_size,             //    kEpsBoostOutV2
    CurrentReading_size,             //    kEpsLoadI2
    CurrentReading_size,             //    kEpsBoostInI2
    VoltageReading_size,             //    kEpsRail1
    VoltageReading_size,             //    kEpsRail2
    VoltageReading_size,             //    kEpsTopPanelV
    VoltageReading_size,             //    kEpsTopPanelI
    VoltageReading_size,             //    kEpsTopSolarV
    CurrentReading_size,             //    kEpsTopSolarI
                                     //    kEpsBmsReadings1
                                     //    kEpsBmsReadings2
    VoltageReading_size,             //    kEpsBmsBatV1
    CurrentReading_size,             //    kEpsBmsBatI1
    VoltageReading_size,             //    kEpsBmsSysV1
    VoltageReading_size,             //    kEpsBmsInV1
    CurrentReading_size,             //    kEpsBmsInI1
                                     //    kEpsBmsJeitaRegion1
                                     //    kEpsBmsSystemStatus1
                                     //    kEpsBmsChargerState1
                                     //    kEpsBmsChargeIndicator1
                                     //    kEpsBmsRechargeThreshold1
                                     //    kEpsBmsChargerConfig1
                                     //    kEpsBmsCxThreshold1
                                     //    kEpsBmsVChargeSet1
                                     //    kEpsBmsIChargeTarget1
                                     //    kEpsBmsConfig1
                                     //    kEpsBmsQCount1
                                     //    kEpsBmsQCountScale1
                                     //    kEpsBmsTelemetryValid1
    VoltageReading_size,             //    kEpsBmsBatV2
    CurrentReading_size,             //    kEpsBmsBatI2
    VoltageReading_size,             //    kEpsBmsSysV2
    VoltageReading_size,             //    kEpsBmsInV2
    CurrentReading_size,             //    kEpsBmsInI2
                                     //    kEpsBmsJeitaRegion2
                                     //    kEpsBmsSystemStatus2
                                     //    kEpsBmsChargerState2
                                     //    kEpsBmsChargeIndicator2
                                     //    kEpsBmsRechargeThreshold2
                                     //    kEpsBmsChargerConfig2
                                     //    kEpsBmsCxThreshold2
                                     //    kEpsBmsVChargeSet2
                                     //    kEpsBmsIChargeTarget2
                                     //    kEpsBmsConfig2
                                     //    kEpsBmsQCount2
                                     //    kEpsBmsQCountScale2
                                     //    kEpsBmsTelemetryValid2
    CurrentReading_size,             //    kFsTorquerXI
    CurrentReading_size,             //    kFsTorquerTotalI
    CurrentReading_size,             //    kFsTorquerYI
    CurrentReading_size,             //    kFsTorquerZI
    VoltageReading_size,             //    kFsTorquerXAV
    VoltageReading_size,             //    kFsTorquerXBV
    VoltageReading_size,             //    kFsTorquerYAV
    VoltageReading_size,             //    kFsTorquerYBV
    VoltageReading_size,             //    kFsTorquerZAV
    VoltageReading_size,             //    kFsTorquerZBV
    TemperatureReading_size,         //    kFsHbXT
    TemperatureReading_size,         //    kFsHbYT
    TemperatureReading_size,         //    kFsHbZT
    GyroscopeReading_size,           //    kFsImuGyro1
    AccelerometerReading_size,       //    kFsImuAccel1
    TemperatureReading_size,         //    kFsImuT1
    MagnetometerReading_size,        //    kFsImuMagno1
    GyroscopeReading_size,           //    kFsImuGyro2
    AccelerometerReading_size,       //    kFsImuAccel2
    TemperatureReading_size,         //    kFsImuT2
    MagnetometerReading_size,        //    kFsImuMagno2
                                     //    kFsRad1
    TemperatureReading_size,         //    kUtilT
    TemperatureReading_size,         //    kCdhT
    TemperatureReading_size,         //    kXPosT1
    TemperatureReading_size,         //    kXPosT2
    TemperatureReading_size,         //    kYPosT1
    TemperatureReading_size,         //    kYPosT2
    TemperatureReading_size,         //    kXNegT1
    TemperatureReading_size,         //    kXNegT2
    TemperatureReading_size,         //    kYNegT1
    TemperatureReading_size,         //    kYNegT2
    TemperatureReading_size,         //    kZNegT1
    TemperatureReading_size,         //    kZNegT2
    TemperatureReading_size,         //    kZPosT
    VoltageReading_size,             //    kXPosV
    CurrentReading_size,             //    kXPosI
    VoltageReading_size,             //    kXPosSolarV
    CurrentReading_size,             //    kXPosSolarI
    VoltageReading_size,             //    kYPosV
    CurrentReading_size,             //    kYPosI
    VoltageReading_size,             //    kYPosSolarV
    CurrentReading_size,             //    kYPosSolarI
    VoltageReading_size,             //    kXNegV
    CurrentReading_size,             //    kXNegI
    VoltageReading_size,             //    kXNegSolarV
    CurrentReading_size,             //    kXNegSolarI
    VoltageReading_size,             //    kYNegV
    CurrentReading_size,             //    kYNegI
    VoltageReading_size,             //    kYNegSolarV
    CurrentReading_size,             //    kYNegSolarI
    VoltageReading_size,             //    kZNegV
    CurrentReading_size,             //    kZNegI
    VoltageReading_size,             //    kZNegSolarV
    CurrentReading_size,             //    kZNegSolarI
                                     //    kXPosSun
                                     //    kXPosIr
                                     //    kXPosRad
                                     //    kYPosSun
                                     //    kYPosIr
                                     //    kXNegSun
                                     //    kXNegIr
                                     //    kYNegSun
                                     //    kYNegIr
                                     //    kZNegSun
                                     //    kZNegIr1
                                     //    kZNegIr2
                                     //    kZPosSun
                                     //    kCdhTime
                                     //    kUtilNtcV1
                                     //    kUtilNtcV2
                                     //    kUtilHeatV
                                     //    kSwFsAngularVelocity1
                                     //    kSwFsAngularVelocity2
                                     //    kSwFsAngularVelocity3
                                     //    kSwFsAttitudeCov
                                     //    kSwFsAttitudeDistance
                                     //    kSwFsAttitudeQuaternion1
                                     //    kSwFsAttitudeQuaternion2
                                     //    kSwFsAttitudeQuaternion3
                                     //    kSwFsAttitudeQuaternion4
                                     //    kSwFsControl1
                                     //    kSwFsControl2
                                     //    kSwFsControl3
                                     //    kSwFsNadir1
                                     //    kSwFsNadir2
                                     //    kSwFsNadir3
                                     //    kSwFsHealthFlags
                                     //    kSwFsAttitudeFlags
                                     //    kSwFsFlags
                                     //    kSwFsLocation
                                     //    kSwFsIr
                                     //    kSwCdhBusFault
                                     //    kSwCdhHeartbeat
                                     //    kSwCdhLastReboot
                                     //    kSwCdhMemory
                                     //    kSwCdhMemoryAvailable
                                     //    kSwCdhRtcStatus
                                     //    kSwCdhMcu1
                                     //    kSwCdhMcu2
                                     //    kSwMcuReCount
                                     //    kConsoleMessage
                                     //    kSdCardDumpMessage
    BmsChargingInfoReading_size,     //    kEpsBmsChargingInfoReading1
    BmsCurrentsReading_size,         //    kEpsBmsCurrentsReading1
    BmsOperationValuesReading_size,  //    kEpsBmsOperationValuesReading1
    BmsSettingsReading_size,         //    kEpsBmsSettingsReading1
    BmsTemperatureReading_size,      //    kEpsBmsTemperatureReading1
    BmsVoltagesReading_size,         //    kEpsBmsVoltagesReading1
    BmsChargingInfoReading_size,     //    kEpsBmsChargingInfoReading2
    BmsCurrentsReading_size,         //    kEpsBmsCurrentsReading2
    BmsOperationValuesReading_size,  //    kEpsBmsOperationValuesReading2
    BmsSettingsReading_size,         //    kEpsBmsSettingsReading2
    BmsTemperatureReading_size,      //    kEpsBmsTemperatureReading2
    BmsVoltagesReading_size,         //    kEpsBmsVoltagesReading2
    //    kTestCurrent
    //    kLithiumTelemetry
    //    kAntennaBurnerInfo
    //    kResetInfoContainer
};

constexpr uint16_t ComputeFileOffset(int measurable_id) {
    return measurable_id == 0 ? 0
                              : ComputeFileOffset(measurable_id - 1) +
                                    kMeasurableReadingSize[measurable_id - 1] *
                                        kCircularBufferMessageLength;
};

constexpr uint16_t kFileOffsets[kMeasurableIdEnd + 1] = {
    ComputeFileOffset(kComInV1),
    ComputeFileOffset(kComOutV1),
    ComputeFileOffset(kComInV2),
    ComputeFileOffset(kComOutV2),
    ComputeFileOffset(kComInI1),
    ComputeFileOffset(kComOutI1),
    ComputeFileOffset(kComInI2),
    ComputeFileOffset(kComOutI2),
    ComputeFileOffset(kComT1),
    ComputeFileOffset(kComT2),
    // ComputeFileOffset(kComRxBytes),
    // ComputeFileOffset(kComTxBytes),
    // ComputeFileOffset(kComAntennaFlags),
    // ComputeFileOffset(kComLithiumOps),
    // ComputeFileOffset(kComLithiumRssi),
    // ComputeFileOffset(kComLithiumT),
    // ComputeFileOffset(kComLithiumTime),
    ComputeFileOffset(kEpsT1),
    ComputeFileOffset(kEpsT2),
    ComputeFileOffset(kEpsBmsDieT1),
    ComputeFileOffset(kEpsBmsDieT2),
    ComputeFileOffset(kEpsBmsBatT1),
    ComputeFileOffset(kEpsBmsBatT2),
    ComputeFileOffset(kEpsAdcBatV1),
    ComputeFileOffset(kEpsBoostOutV1),
    ComputeFileOffset(kEpsBoostInI1),
    ComputeFileOffset(kEpsLoadI1),
    ComputeFileOffset(kEpsAdcBatV2),
    ComputeFileOffset(kEpsBoostOutV2),
    ComputeFileOffset(kEpsLoadI2),
    ComputeFileOffset(kEpsBoostInI2),
    ComputeFileOffset(kEpsRail1),
    ComputeFileOffset(kEpsRail2),
    ComputeFileOffset(kEpsTopPanelV),
    ComputeFileOffset(kEpsTopPanelI),
    ComputeFileOffset(kEpsTopSolarV),
    ComputeFileOffset(kEpsTopSolarI),
    // ComputeFileOffset(kEpsBmsReadings1),
    // ComputeFileOffset(kEpsBmsReadings2),
    ComputeFileOffset(kEpsBmsBatV1),
    ComputeFileOffset(kEpsBmsBatI1),
    ComputeFileOffset(kEpsBmsSysV1),
    ComputeFileOffset(kEpsBmsInV1),
    ComputeFileOffset(kEpsBmsInI1),
    // ComputeFileOffset(kEpsBmsJeitaRegion1),
    // ComputeFileOffset(kEpsBmsSystemStatus1),
    // ComputeFileOffset(kEpsBmsChargerState1),
    // ComputeFileOffset(kEpsBmsChargeIndicator1),
    // ComputeFileOffset(kEpsBmsRechargeThreshold1),
    // ComputeFileOffset(kEpsBmsChargerConfig1),
    // ComputeFileOffset(kEpsBmsCxThreshold1),
    // ComputeFileOffset(kEpsBmsVChargeSet1),
    // ComputeFileOffset(kEpsBmsIChargeTarget1),
    // ComputeFileOffset(kEpsBmsConfig1),
    // ComputeFileOffset(kEpsBmsQCount1),
    // ComputeFileOffset(kEpsBmsQCountScale1),
    // ComputeFileOffset(kEpsBmsTelemetryValid1),
    ComputeFileOffset(kEpsBmsBatV2),
    ComputeFileOffset(kEpsBmsBatI2),
    ComputeFileOffset(kEpsBmsSysV2),
    ComputeFileOffset(kEpsBmsInV2),
    ComputeFileOffset(kEpsBmsInI2),
    // ComputeFileOffset(kEpsBmsJeitaRegion2),
    // ComputeFileOffset(kEpsBmsSystemStatus2),
    // ComputeFileOffset(kEpsBmsChargerState2),
    // ComputeFileOffset(kEpsBmsChargeIndicator2),
    // ComputeFileOffset(kEpsBmsRechargeThreshold2),
    // ComputeFileOffset(kEpsBmsChargerConfig2),
    // ComputeFileOffset(kEpsBmsCxThreshold2),
    // ComputeFileOffset(kEpsBmsVChargeSet2),
    // ComputeFileOffset(kEpsBmsIChargeTarget2),
    // ComputeFileOffset(kEpsBmsConfig2),
    // ComputeFileOffset(kEpsBmsQCount2),
    // ComputeFileOffset(kEpsBmsQCountScale2),
    // ComputeFileOffset(kEpsBmsTelemetryValid2),
    ComputeFileOffset(kFsTorquerXI),
    ComputeFileOffset(kFsTorquerTotalI),
    ComputeFileOffset(kFsTorquerYI),
    ComputeFileOffset(kFsTorquerZI),
    ComputeFileOffset(kFsTorquerXAV),
    ComputeFileOffset(kFsTorquerXBV),
    ComputeFileOffset(kFsTorquerYAV),
    ComputeFileOffset(kFsTorquerYBV),
    ComputeFileOffset(kFsTorquerZAV),
    ComputeFileOffset(kFsTorquerZBV),
    ComputeFileOffset(kFsHbXT),
    ComputeFileOffset(kFsHbYT),
    ComputeFileOffset(kFsHbZT),
    ComputeFileOffset(kFsImuGyro1),
    ComputeFileOffset(kFsImuAccel1),
    ComputeFileOffset(kFsImuT1),
    ComputeFileOffset(kFsImuMagno1),
    ComputeFileOffset(kFsImuGyro2),
    ComputeFileOffset(kFsImuAccel2),
    ComputeFileOffset(kFsImuT2),
    ComputeFileOffset(kFsImuMagno2),
    ComputeFileOffset(kFsRad1),
    ComputeFileOffset(kUtilT),
    ComputeFileOffset(kCdhT),
    ComputeFileOffset(kXPosT1),
    ComputeFileOffset(kXPosT2),
    ComputeFileOffset(kYPosT1),
    ComputeFileOffset(kYPosT2),
    ComputeFileOffset(kXNegT1),
    ComputeFileOffset(kXNegT2),
    ComputeFileOffset(kYNegT1),
    ComputeFileOffset(kYNegT2),
    ComputeFileOffset(kZNegT1),
    ComputeFileOffset(kZNegT2),
    ComputeFileOffset(kZPosT),
    ComputeFileOffset(kXPosV),
    ComputeFileOffset(kXPosI),
    ComputeFileOffset(kXPosSolarV),
    ComputeFileOffset(kXPosSolarI),
    ComputeFileOffset(kYPosV),
    ComputeFileOffset(kYPosI),
    ComputeFileOffset(kYPosSolarV),
    ComputeFileOffset(kYPosSolarI),
    ComputeFileOffset(kXNegV),
    ComputeFileOffset(kXNegI),
    ComputeFileOffset(kXNegSolarV),
    ComputeFileOffset(kXNegSolarI),
    ComputeFileOffset(kYNegV),
    ComputeFileOffset(kYNegI),
    ComputeFileOffset(kYNegSolarV),
    ComputeFileOffset(kYNegSolarI),
    ComputeFileOffset(kZNegV),
    ComputeFileOffset(kZNegI),
    ComputeFileOffset(kZNegSolarV),
    ComputeFileOffset(kZNegSolarI),
    ComputeFileOffset(kXPosSun),
    ComputeFileOffset(kXPosIr),
    ComputeFileOffset(kXPosRad),
    ComputeFileOffset(kYPosSun),
    ComputeFileOffset(kYPosIr),
    ComputeFileOffset(kXNegSun),
    ComputeFileOffset(kXNegIr),
    ComputeFileOffset(kYNegSun),
    ComputeFileOffset(kYNegIr),
    ComputeFileOffset(kZNegSun),
    ComputeFileOffset(kZNegIr1),
    ComputeFileOffset(kZNegIr2),
    ComputeFileOffset(kZPosSun),
    ComputeFileOffset(kCdhTime),
    // ComputeFileOffset(kUtilNtcV1),
    // ComputeFileOffset(kUtilNtcV2),
    // ComputeFileOffset(kUtilHeatV),
    // ComputeFileOffset(kSwFsAngularVelocity1),
    // ComputeFileOffset(kSwFsAngularVelocity2),
    // ComputeFileOffset(kSwFsAngularVelocity3),
    // ComputeFileOffset(kSwFsAttitudeCov),
    // ComputeFileOffset(kSwFsAttitudeDistance),
    // ComputeFileOffset(kSwFsAttitudeQuaternion1),
    // ComputeFileOffset(kSwFsAttitudeQuaternion2),
    // ComputeFileOffset(kSwFsAttitudeQuaternion3),
    // ComputeFileOffset(kSwFsAttitudeQuaternion4),
    // ComputeFileOffset(kSwFsControl1),
    // ComputeFileOffset(kSwFsControl2),
    // ComputeFileOffset(kSwFsControl3),
    // ComputeFileOffset(kSwFsNadir1),
    // ComputeFileOffset(kSwFsNadir2),
    // ComputeFileOffset(kSwFsNadir3),
    // ComputeFileOffset(kSwFsHealthFlags),
    // ComputeFileOffset(kSwFsAttitudeFlags),
    // ComputeFileOffset(kSwFsFlags),
    // ComputeFileOffset(kSwFsLocation),
    // ComputeFileOffset(kSwFsIr),
    // ComputeFileOffset(kSwCdhBusFault),
    // ComputeFileOffset(kSwCdhHeartbeat),
    // ComputeFileOffset(kSwCdhLastReboot),
    // ComputeFileOffset(kSwCdhMemory),
    // ComputeFileOffset(kSwCdhMemoryAvailable),
    // ComputeFileOffset(kSwCdhRtcStatus),
    // ComputeFileOffset(kSwCdhMcu1),
    // ComputeFileOffset(kSwCdhMcu2),
    // ComputeFileOffset(kSwMcuReCount),
    // ComputeFileOffset(kConsoleMessage),
    // ComputeFileOffset(kSdCardDumpMessage),
    ComputeFileOffset(kEpsBmsChargingInfoReading1),
    ComputeFileOffset(kEpsBmsCurrentsReading1),
    ComputeFileOffset(kEpsBmsOperationValuesReading1),
    ComputeFileOffset(kEpsBmsSettingsReading1),
    ComputeFileOffset(kEpsBmsTemperatureReading1),
    ComputeFileOffset(kEpsBmsVoltagesReading1),
    ComputeFileOffset(kEpsBmsChargingInfoReading2),
    ComputeFileOffset(kEpsBmsCurrentsReading2),
    ComputeFileOffset(kEpsBmsOperationValuesReading2),
    ComputeFileOffset(kEpsBmsSettingsReading2),
    ComputeFileOffset(kEpsBmsTemperatureReading2),
    ComputeFileOffset(kEpsBmsVoltagesReading2),
    // ComputeFileOffset(kTestCurrent),
    // ComputeFileOffset(kLithiumTelemetry),
    // ComputeFileOffset(kAntennaBurnerInfo),
    // ComputeFileOffset(kResetInfoContainer),
    // Final offset for the test circular buffer
    ComputeFileOffset(kMeasurableIdEnd),
};

#endif  //  SRC_DATABASE_FILE_OFFSETS_H_
