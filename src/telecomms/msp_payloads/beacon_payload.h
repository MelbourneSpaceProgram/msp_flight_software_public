#ifndef SRC_TELECOMMS_MSP_PAYLOADS_BEACON_PAYLOAD_H_
#define SRC_TELECOMMS_MSP_PAYLOADS_BEACON_PAYLOAD_H_

#include <external/etl/array.h>
#include <math.h>
#include <src/messages/Time.pb.h>
#include <src/messages/message.h>
#include <src/telecomms/msp_payloads/transmit_payload.h>
#include <src/util/data_types.h>
#include <stdlib.h>

class BeaconPayload : public TransmitPayload {
   public:
    static const uint8_t kFsHealthFlagByteCount = 20;
    static const uint8_t kFsFsAttitudeFlagByteCount = 2;
    static const uint8_t kFsLocationFloatCount = 2;

    BeaconPayload();
    SerialisedMessage SerialiseTo(byte* serial_buffer) const;
    uint16_t GetSerialisedSize() const;
    uint8_t GetPayloadCode() const;

    // TODO(dingbenjamin): Change autogenned args to snake case
    BeaconPayload* SetCommsRegulator1OutputCurrent(
        float commsRegulator1OutputCurrent);
    BeaconPayload* SetCommsRegulator1OutputVoltage(
        float commsRegulator1OutputVoltage);
    BeaconPayload* SetCommsRegulator2OutputCurrent(
        float commsRegulator2OutputCurrent);
    BeaconPayload* SetCommsRegulator2OutputVoltage(
        float commsRegulator2OutputVoltage);
    BeaconPayload* SetCommsTemp2(byte commsTemp2);
    BeaconPayload* SetPowerBatVoltage1(float powerBatVoltage1);
    BeaconPayload* SetPowerLoadCurrent1(float powerLoadCurrent1);
    BeaconPayload* SetPowerBatVoltage2(float powerBatVoltage2ADC);
    BeaconPayload* SetPowerLoadCurrent2(float powerLoadCurrent2);
    BeaconPayload* SetPower5VRail1(float power5VRail1);
    BeaconPayload* SetPower5VRail2(float power5VRail2);
    BeaconPayload* SetPowerTopPanelVoltage(float powerTopPanelVoltage);
    BeaconPayload* SetPowerTopPanelCurrent(float powerTopPanelCurrent);
    BeaconPayload* SetPowerTemp1(byte powerTemp1);
    BeaconPayload* SetPowerTemp2(byte powerTemp2);
    BeaconPayload* SetPowerPanelVoltageAccessXAxis(
        float powerPanelVoltageAccessXAxis);
    BeaconPayload* SetPowerPanelCurrenteAccessXAxis(
        float powerPanelCurrentAccessXAxis);
    BeaconPayload* SetPowerSolarVoltageAccessXAxis(
        float powerSolarVoltageAccessXAxis);
    BeaconPayload* SetPowerSolarCurrentAccessXAxis(
        float powerSolarCurrentAccessXAxis);
    BeaconPayload* SetPowerAccessPanelXAxisTemp1(
        byte powerAccessPanelXAxisTemp1);
    BeaconPayload* SetPowerAccessPanelXAxisTemp2(
        byte powerAccessPanelXAxisTemp2);
    BeaconPayload* SetFsRadiationAccessPanelXAxis(
        byte fsRadiationAccessPanelXAxis);
    BeaconPayload* SetPowerPanelVoltageSide1YAxis(
        float powerPanelVoltageSide1YAxis);
    BeaconPayload* SetPowerPanelCurrentSide1YAxis(
        float powerPanelCurrentSide1YAxis);
    BeaconPayload* SetPowerSolarVoltageSide1YAxis(
        float powerSolarVoltageSide1YAxis);
    BeaconPayload* SetPowerSolarCurrentSide1YAxis(
        float powerSolarCurrentSide1YAxis);
    BeaconPayload* SetPowerSide1PanelTemp1YAxis(byte powerSide1PanelTemp1YAxis);
    BeaconPayload* SetPowerSide1PanelYAxisTemp2(byte powerSide1PanelYAxisTemp2);
    BeaconPayload* SetPowerPanelVoltageSide2MinusXAxis(
        float powerPanelVoltageSide2MinusXAxis);
    BeaconPayload* SetPowerPanelCurrentSide2MinusXAxis(
        float powerPanelCurrentSide2MinusXAxis);
    BeaconPayload* SetPowerSolarVoltageSide2MinusXAxis(
        float powerSolarVoltageSide2MinusXAxis);
    BeaconPayload* SetPowerSolarCurrentSide2MinusXAxis(
        float powerSolarCurrentSide2MinusXAxis);
    BeaconPayload* SetPowerSide2PanelMinusXAxisTemp1(
        byte powerSide2PanelMinusXAxisTemp1);
    BeaconPayload* SetPowerSide2PanelMinusXAxisTemp2(
        byte powerSide2PanelMinusXAxisTemp2);
    BeaconPayload* SetPowerPanelVoltageSide3MinusYAxis(
        float powerPanelVoltageSide3MinusYAxis);
    BeaconPayload* SetPowerPanelCurrentSide3MinusYAxis(
        float powerPanelCurrentSide3MinusYAxis);
    BeaconPayload* SetPowerSolarVoltageSide3MinusYAxis(
        float powerSolarVoltageSide3MinusYAxis);
    BeaconPayload* SetPowerSolarCurrentSide3MinusYAxis(
        float powerSolarCurrentSide3MinusYAxis);
    BeaconPayload* SetPowerSide3PanelMinusYAxisTemp1(
        byte powerSide3PanelMinusYAxisTemp1);
    BeaconPayload* SetPowerSide3PanelMinusYAxisTemp2(
        byte powerSide3PanelMinusYAxisTemp2);
    BeaconPayload* SetPowerPanelVoltageBottomMinusZAxis(
        float powerPanelVoltageBottomMinusZAxis);
    BeaconPayload* SetPowerPanelCurrentBottomMinusZAxis(
        float powerPanelCurrentBottomMinusZAxis);
    BeaconPayload* SetPowerSolarVoltageBottomMinusZAxis(
        float powerSolarVoltageBottomMinusZAxis);
    BeaconPayload* SetPowerSolarCurrentBottomMinusZAxis(
        float powerSolarCurrentBottomMinusZAxis);
    BeaconPayload* SetPowerBottomPanelMinusZAxisTemp1(
        byte powerBottomPanelMinusZAxisTemp1);
    BeaconPayload* SetPowerBottomPanelMinusZAxisTemp2(
        byte powerBottomPanelMinusZAxisTemp2);
    BeaconPayload* SetPowerTopPanelZAxisTemp1(byte powerTopPanelZAxisTemp1);
    BeaconPayload* SetUtilitiesNtc1(byte utilitiesNTC1);
    BeaconPayload* SetUtilitiesNtc2(uint16_t utilitiesNTC2);
    BeaconPayload* SetUtilitiesHeating(uint16_t utilitiesHeating);
    BeaconPayload* SetUtilitiesTemp(byte utilitiesTemp);
    BeaconPayload* SetFsTorquerCurrentX(float fsTorquerCurrentX);
    BeaconPayload* SetFsTorquerCurrentY(float fsTorquerCurrentY);
    BeaconPayload* SetFsTorquerCurrentZ(float fsTorquerCurrentZ);
    BeaconPayload* SetFsTempHbX(byte fsTempHbX);
    BeaconPayload* SetFsTempHbY(byte fsTempHbY);
    BeaconPayload* SetFsTempHbZ(byte fsTempHbZ);
    BeaconPayload* SetFsRadiation1(uint16_t fsRadiation1);
    BeaconPayload* SetCdhSysTemp(byte cdhSysTemp);
    BeaconPayload* SetCommsBytesReceived(uint16_t commsBytesReceived);
    BeaconPayload* SetCommsBytesTransmitted(uint16_t commsBytesTransmitted);
    BeaconPayload* SetCommsAntennaFlags(uint16_t commsAntennaFlags);
    BeaconPayload* SetCommsLithiumOpCount(byte commsLithiumOpCount);
    BeaconPayload* SetCommsLithiumRssi(uint16_t commsLithiumRssi);
    BeaconPayload* SetCommsLithiumTemp(uint16_t commsLithiumTemp);
    BeaconPayload* SetCommsLithiumTime(Time commsLithiumTime);
    BeaconPayload* SetFsIr(uint8_t fsIr);
    BeaconPayload* SetFsAngularVelocity1(float fsAngularVelocity1);
    BeaconPayload* SetFsAngularVelocity2(float fsAngularVelocity2);
    BeaconPayload* SetFsAngularVelocity3(float fsAngularVelocity3);
    BeaconPayload* SetFsAttitudeCov(uint8_t fsAttitudeCov);
    BeaconPayload* SetFsAttitudeDistance(uint8_t fsAttitudeDistance);
    BeaconPayload* SetFsAttitudeFlags(
        etl::array<byte, kFsFsAttitudeFlagByteCount> fsAttitudeFlags);
    BeaconPayload* SetFsAttitudeQuaternion1(float fsAttitudeQuaternion1);
    BeaconPayload* SetFsAttitudeQuaternion2(float fsAttitudeQuaternion2);
    BeaconPayload* SetFsAttitudeQuaternion3(float fsAttitudeQuaternion3);
    BeaconPayload* SetFsAttitudeQuaternion4(float fsAttitudeQuaternion4);
    BeaconPayload* SetFsControl1(uint8_t fsControl1);
    BeaconPayload* SetFsControl2(uint8_t fsControl2);
    BeaconPayload* SetFsControl3(uint8_t fsControl3);
    BeaconPayload* SetFsFlags(byte fsFlags);
    BeaconPayload* SetFsHealthFlags(
        etl::array<byte, kFsHealthFlagByteCount> fs_health_flags);
    BeaconPayload* SetMcuResetCount1(uint8_t mcuResetCount1);
    BeaconPayload* SetFsLocation(
        etl::array<float, kFsLocationFloatCount> fsLocation);
    BeaconPayload* SetCdhMemoryAvailable(uint16_t cdhMemoryAvailable);
    BeaconPayload* SetCdhLastReboot(const Time& cdhLastReboot);
    BeaconPayload* SetCdhMemory(uint16_t cdhMemory);

    float ConstrainToRange(float data, uint8_t upper_bound);
    static int16_t ScaleCurrentMeasurable(float data);
    static int16_t ScaleVoltageMeasurable(float data_types);
    static byte ScaleTempMeasurable(float data);

   private:
    static const uint8_t kOutreachMessageSize = 27;
    int16_t comms_regulator_1_output_current;
    int16_t comms_regulator_1_output_voltage;
    int16_t comms_regulator_2_output_current;
    int16_t comms_regulator_2_output_voltage;
    byte comms_temp_2;
    int16_t power_bat_voltage_1;
    int16_t power_load_current_1;
    int16_t power_bat_voltage_2;
    int16_t power_load_current_2;
    int16_t power_5v_rail_1;  //[-20, 20]
    int16_t power_5v_rail_2;
    int16_t power_top_panel_voltage;
    int16_t power_top_panel_current;
    byte power_temp_1;
    byte power_temp_2;
    int16_t power_panel_voltage_access_x_axis;
    int16_t power_panel_current_access_x_axis;
    int16_t power_solar_voltage_access_x_axis;
    int16_t power_solar_current_access_x_axis;
    byte power_access_panel_x_axis_temp_1;
    byte power_access_panel_x_axis_temp_2;
    byte fs_radiation_access_panel_x_axis;
    int16_t power_panel_voltage_side_1_y_axis;
    int16_t power_panel_current_side_1_y_axis;
    int16_t power_solar_voltage_side_1_y_axis;
    int16_t power_solar_current_side_1_y_axis;
    byte power_side_1_panel_y_axis_temp_1;
    byte power_side_1_panel_y_axis_temp_2;
    int16_t power_panel_voltage_side_2_minus_x_axis;
    int16_t power_panel_current_side_2_minus_x_axis;
    int16_t power_solar_voltage_side_2_minus_x_axis;
    int16_t power_solar_current_side_2_minus_x_axis;
    byte power_side_2_panel_minus_x_axis_temp_1;
    byte power_side_2_panel_minus_x_axis_temp_2;
    int16_t power_panel_voltage_side_3_minus_y_axis;
    int16_t power_panel_current_side_3_minus_y_axis;
    int16_t power_solar_voltage_side_3_minus_y_axis;
    int16_t power_solar_current_side_3_minus_y_axis;
    byte power_side_3_panel_minus_y_axis_temp_1;
    byte power_side_3_panel_minus_y_axis_temp_2;
    int16_t power_panel_voltage_bottom_minus_z_axis;
    int16_t power_panel_current_bottom_minus_z_axis;
    int16_t power_solar_voltage_bottom_minus_z_axis;
    int16_t power_solar_current_bottom_minus_z_axis;
    byte power_bottom_panel_minus_z_axis_temp_1;
    byte power_bottom_panel_minus_z_axis_temp_2;
    byte power_top_panel_z_axis_temp_1;
    uint16_t util_ntc_1;    // TODO(naverill) clarify data type
    uint16_t util_ntc_2;    // TODO(naverill) clarify data type
    uint16_t util_heating;  // TODO(naverill) clarify data type
    byte util_temp;
    int16_t fs_torquer_current_x;
    int16_t fs_torquer_current_y;
    int16_t fs_torquer_current_z;
    byte fs_temp_hb_x;
    byte fs_temp_hb_y;
    byte fs_temp_hb_z;
    uint16_t fs_radiation_1;  // TODO(naverill) clarify data type
    byte cdh_sys_temp;
    uint32_t comms_bytes_received;
    uint32_t comms_bytes_transmitted;
    byte comms_antenna_flags;
    uint16_t comms_lithium_op_count;
    uint8_t comms_lithium_rssi;
    byte comms_lithium_temp;
    Time comms_lithium_time;
    float fs_angular_velocity_1;  // TODO(naverill) clarify data type
    float fs_angular_velocity_2;  // TODO(naverill) clarify data type
    float fs_angular_velocity_3;  // TODO(naverill) clarify data type
    uint8_t fs_attitude_cov;
    uint8_t fs_attitude_distance;
    float fs_attitude_quaternion_1;  // TODO(naverill) clarify data type
    float fs_attitude_quaternion_2;  // TODO(naverill) clarify data type
    float fs_attitude_quaternion_3;  // TODO(naverill) clarify data type
    float fs_attitude_quaternion_4;  // TODO(naverill) clarify data type
    uint8_t fs_control_1;
    uint8_t fs_control_2;
    uint8_t fs_control_3;
    etl::array<byte, kFsFsAttitudeFlagByteCount> fs_attitude_flags;
    etl::array<byte, kFsHealthFlagByteCount> fs_health_flags;
    byte fs_flags;
    etl::array<float, kFsLocationFloatCount> fs_location;
    uint8_t fs_ir;
    uint16_t cdh_mcu_1;
    uint8_t mcu_reset_count_1;
    uint16_t cdh_memory;
    uint16_t cdh_memory_available;
    Time cdh_last_reboot;
    char* comms_outreach;  // 27 bytes

    static const uint8_t kVoltageUpperBound =
        20;  // assumed [-20, 20] value range
    static const uint8_t kCurrentUpperBound =
        20;  // assumed [-20, 20] value range
    static const uint8_t kTempUpperBound =
        128;  // assumed [-128, 128] value range
    static const uint16_t kVoltageScaleFactor =
        2 * pow(sizeof(int16_t) - 1) / kVoltageUpperBound;
    static const uint16_t kCurrentScaleFactor =
        2 * pow(sizeof(int16_t) - 1) / kCurrentUpperBound;
    static const uint16_t kTempScaleFactor =
        2 * pow(sizeof(int16_t) - 1) / kTempUpperBound;
};

#endif  // SRC_TELECOMMS_MSP_PAYLOADS_BEACON_PAYLOAD_H_
