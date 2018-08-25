#ifndef SRC_TELECOMMS_MSP_PAYLOADS_BEACON_PAYLOAD_H_
#define SRC_TELECOMMS_MSP_PAYLOADS_BEACON_PAYLOAD_H_

#include <external/etl/array.h>
#include <src/messages/Time.pb.h>
#include <src/messages/message.h>
#include <src/telecomms/msp_payloads/transmit_payload.h>
#include <src/util/data_types.h>

class BeaconPayload : public TransmitPayload {
   public:
    BeaconPayload();
    SerialisedMessage SerialiseTo(byte* serial_buffer) const;
    uint16_t GetSerialisedSize() const;
    uint8_t GetPayloadCode() const;

    int16_t comms_regulator_1_output_current;
    float comms_regulator_1_output_voltage;
    float comms_regulator_2_output_current;
    float comms_regulator_2_output_voltage;
    float comms_temp_2;
    float power_bat_voltage_1;
    float power_load_current_1;
    float power_bat_voltage_2;
    float power_load_current_2;
    float power_5v_rail_1;
    float power_5v_rail_2;
    float power_top_panel_voltage;
    float power_top_panel_current;
    float power_temp_1;
    float power_temp_2;
    float power_panel_voltage_access_x_axis;
    float power_panel_current_access_x_axis;
    float power_access_panel_x_axis_temp_1;
    float fs_radiation_access_panel_x_axis;
    float power_panel_voltage_side_1_y_axis;
    float power_panel_current_side_1_y_axis;
    float power_side_1_panel_temp_1_y_axis_;
    float power_panel_voltage_side_2_minus_x_axis;
    float power_panel_current_side_2_minus_x_axis;
    float power_side_2_panel_minus_x_axis_temp_1;
    float power_panel_voltage_side_3_minus_y_axis;
    float power_panel_current_side_3_minus_y_axis;
    float power_side_3_panel_minus_y_axis_temp_1;
    float power_panel_voltage_bottom_minus_z_axis;
    float power_panel_current_bottom_minus_z_axis;
    float power_solar_voltage_bottom_minus_z_axis;
    float power_solar_current_bottom_minus_z_axis;
    float power_bottom_panel_minus_z_axis_temp_1;
    float power_top_panel_z_axis_temp_1;
    uint16_t util_ntc_1;    // TODO(naverill) clarify data type
    uint16_t util_ntc_2;    // TODO(naverill) clarify data type
    uint16_t util_heating;  // TODO(naverill) clarify data type
    float util_temp;
    float fs_torquer_current_x;
    float fs_torquer_current_y;
    float fs_torquer_current_z;
    float fs_temp_hb_x;
    float fs_temp_hb_y;
    float fs_temp_hb_z;
    uint16_t fs_radiation_1;  // TODO(naverill) clarify data type
    float cdh_sys_temp;
    uint16_t comms_bytes_received;
    uint16_t comms_bytes_transmitted;
    uint16_t comms_antenna_flags;
    uint16_t comms_lithium_op_count;
    uint16_t comms_lithium_rssi;
    uint16_t comms_lithium_temp;
    Time comms_lithium_time;
    int16_t fs_angular_velocity_1;  // TODO(naverill) clarify data type
    int16_t fs_angular_velocity_2;  // TODO(naverill) clarify data type
    int16_t fs_angular_velocity_3;  // TODO(naverill) clarify data type
    int16_t fs_attitude_cov;
    int16_t fs_attitude_distance;
    int16_t fs_attitude_quaternion_1;  // TODO(naverill) clarify data type
    int16_t fs_attitude_quaternion_2;  // TODO(naverill) clarify data type
    int16_t fs_attitude_quaternion_3;  // TODO(naverill) clarify data type
    int16_t fs_attitude_quaternion_4;  // TODO(naverill) clarify data type
    int16_t fs_control_1;
    int16_t fs_control_2;
    int16_t fs_control_3;
    uint16_t fs_health_flags;
    uint16_t fs_attitude_flags;
    uint16_t fs_flags;
    int16_t fs_location;
    int16_t fs_ir;
    Time cdh_last_reboot;
    uint16_t cdh_memory;
    uint16_t cdh_memory_available;
    uint16_t cdh_mcu_1;
    uint16_t mcu_reset_count_1;
    char* comms_outreach;  // TODO(dingbenjamin): Convert to etl::string

    static const uint8_t kFsHealthFlagByteCount = 20;
    static const uint8_t kFsFsAttitudeFlagByteCount = 2;
    static const uint8_t kFsLocationFloatCount = 2;
    static const uint8_t kOutreachMessageSize = 27;

    static const uint8_t kVoltageUpperBound =
        20;  // assumed [-20, 20] value range
    static const uint8_t kCurrentUpperBound =
        20;  // assumed [-20, 20] value range
    static const uint8_t kTempUpperBound =
        128;  // assumed [-128, 128] value range

    // TODO(dingbenjamin): Make these private
    static int16_t ScaleArbitraryInt16(float data, uint16_t abs_max);
    static int16_t ScaleCurrent(float data);
    static int16_t ScaleVoltage(float data);
    static int16_t ScaleTemp(float data);
    static float ConstrainToRange(float data, uint16_t abs_max);
};

#endif  // SRC_TELECOMMS_MSP_PAYLOADS_BEACON_PAYLOAD_H_
