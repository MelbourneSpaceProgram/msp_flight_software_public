#include <math.h>
#include <src/messages/serialised_message.h>
#include <src/messages/serialised_message_builder.h>
#include <src/telecomms/msp_payloads/beacon_payload.h>
#include <src/util/data_types.h>
#include <src/util/message_codes.h>

BeaconPayload::BeaconPayload()
    : comms_regulator_1_output_current(kInvalidDouble),
      comms_regulator_1_output_voltage(kInvalidDouble),
      comms_regulator_2_output_current(kInvalidDouble),
      comms_regulator_2_output_voltage(kInvalidDouble),
      comms_temp_2(kInvalidDouble),
      power_bat_voltage_1(kInvalidDouble),
      power_load_current_1(kInvalidDouble),
      power_bat_voltage_2(kInvalidDouble),
      power_load_current_2(kInvalidDouble),
      power_5v_rail_1(kInvalidDouble),
      power_5v_rail_2(kInvalidDouble),
      power_top_panel_voltage(kInvalidDouble),
      power_top_panel_current(kInvalidDouble),
      power_temp_1(kInvalidDouble),
      power_temp_2(kInvalidDouble),
      power_panel_voltage_access_x_axis(kInvalidDouble),
      power_panel_current_access_x_axis(kInvalidDouble),
      power_access_panel_x_axis_temp_1(kInvalidDouble),
      fs_radiation_access_panel_x_axis(kInvalidDouble),
      power_panel_voltage_side_1_y_axis(kInvalidDouble),
      power_panel_current_side_1_y_axis(kInvalidDouble),
      power_panel_voltage_side_2_minus_x_axis(kInvalidDouble),
      power_panel_current_side_2_minus_x_axis(kInvalidDouble),
      power_side_2_panel_minus_x_axis_temp_1(kInvalidDouble),
      power_panel_voltage_side_3_minus_y_axis(kInvalidDouble),
      power_panel_current_side_3_minus_y_axis(kInvalidDouble),
      power_side_3_panel_minus_y_axis_temp_1(kInvalidDouble),
      power_panel_voltage_bottom_minus_z_axis(kInvalidDouble),
      power_panel_current_bottom_minus_z_axis(kInvalidDouble),
      power_solar_voltage_bottom_minus_z_axis(kInvalidDouble),
      power_solar_current_bottom_minus_z_axis(kInvalidDouble),
      power_bottom_panel_minus_z_axis_temp_1(kInvalidDouble),
      power_top_panel_z_axis_temp_1(kInvalidDouble),
      util_ntc_1(0),
      util_ntc_2(0),
      util_heating(0),
      util_temp(kInvalidDouble),
      fs_torquer_current_x(kInvalidDouble),
      fs_torquer_current_y(kInvalidDouble),
      fs_torquer_current_z(kInvalidDouble),
      fs_temp_hb_x(kInvalidDouble),
      fs_temp_hb_y(kInvalidDouble),
      fs_temp_hb_z(kInvalidDouble),
      fs_radiation_1(0),
      cdh_sys_temp(kInvalidDouble),
      comms_bytes_received(0),
      comms_bytes_transmitted(0),
      comms_antenna_flags(0),
      comms_lithium_op_count(0),
      comms_lithium_rssi(0),
      comms_lithium_temp(0),
      comms_lithium_time({0, false}),
      fs_angular_velocity_1(0),
      fs_angular_velocity_2(0),
      fs_angular_velocity_3(0),
      fs_attitude_cov(0),
      fs_attitude_distance(0),
      fs_attitude_quaternion_1(0),
      fs_attitude_quaternion_2(0),
      fs_attitude_quaternion_3(0),
      fs_attitude_quaternion_4(0),
      fs_control_1(0),
      fs_control_2(0),
      fs_control_3(0),
      fs_ir(0),
      cdh_mcu_1(0),
      mcu_reset_count_1(0),
      cdh_memory(0),
      cdh_memory_available(0),
      cdh_last_reboot({0, false}),
      comms_outreach("Hello from Melbourne       ") {}
SerialisedMessage BeaconPayload::SerialiseTo(byte* serial_buffer) const {
    SerialisedMessageBuilder builder(serial_buffer, GetSerialisedSize());

    builder.AddData<int16_t>(ScaleCurrent(comms_regulator_1_output_current))
        .AddData<int16_t>(ScaleVoltage(comms_regulator_1_output_voltage))
        .AddData<int16_t>(ScaleCurrent(comms_regulator_2_output_current))
        .AddData<int16_t>(ScaleVoltage(comms_regulator_2_output_voltage))
        .AddData<int16_t>(ScaleTemp(comms_temp_2))
        .AddData<int16_t>(ScaleVoltage(power_bat_voltage_1))
        .AddData<int16_t>(ScaleCurrent(power_load_current_1))
        .AddData<int16_t>(ScaleVoltage(power_bat_voltage_2))
        .AddData<int16_t>(ScaleCurrent(power_load_current_2))
        .AddData<int16_t>(ScaleVoltage(power_5v_rail_1))
        .AddData<int16_t>(ScaleVoltage(power_5v_rail_2))
        .AddData<int16_t>(ScaleVoltage(power_top_panel_voltage))
        .AddData<int16_t>(ScaleCurrent(power_top_panel_current))
        .AddData<int16_t>(ScaleTemp(power_temp_1))
        .AddData<int16_t>(ScaleTemp(power_temp_2))
        .AddData<int16_t>(ScaleVoltage(power_panel_voltage_access_x_axis))
        .AddData<int16_t>(ScaleCurrent(power_panel_current_access_x_axis))
        .AddData<int16_t>(ScaleTemp(power_access_panel_x_axis_temp_1))
        .AddData<int16_t>(fs_radiation_access_panel_x_axis)
        .AddData<int16_t>(ScaleVoltage(power_panel_voltage_side_1_y_axis))
        .AddData<int16_t>(ScaleCurrent(power_panel_current_side_1_y_axis))
        .AddData<int16_t>(ScaleVoltage(power_panel_voltage_side_2_minus_x_axis))
        .AddData<int16_t>(ScaleCurrent(power_panel_current_side_2_minus_x_axis))
        .AddData<int16_t>(ScaleTemp(power_side_2_panel_minus_x_axis_temp_1))
        .AddData<int16_t>(ScaleVoltage(power_panel_voltage_side_3_minus_y_axis))
        .AddData<int16_t>(ScaleCurrent(power_panel_current_side_3_minus_y_axis))
        .AddData<int16_t>(ScaleTemp(power_side_3_panel_minus_y_axis_temp_1))
        .AddData<int16_t>(ScaleVoltage(power_panel_voltage_bottom_minus_z_axis))
        .AddData<int16_t>(ScaleCurrent(power_panel_current_bottom_minus_z_axis))
        .AddData<int16_t>(ScaleVoltage(power_solar_voltage_bottom_minus_z_axis))
        .AddData<int16_t>(ScaleCurrent(power_solar_current_bottom_minus_z_axis))
        .AddData<int16_t>(ScaleTemp(power_bottom_panel_minus_z_axis_temp_1))
        .AddData<int16_t>(ScaleTemp(power_top_panel_z_axis_temp_1))
        .AddData<int16_t>(util_ntc_1)
        .AddData<int16_t>(util_ntc_2)
        .AddData<int16_t>(util_heating)
        .AddData<int16_t>(ScaleTemp(util_temp))
        .AddData<int16_t>(ScaleCurrent(fs_torquer_current_x))
        .AddData<int16_t>(ScaleCurrent(fs_torquer_current_y))
        .AddData<int16_t>(ScaleCurrent(fs_torquer_current_z))
        .AddData<int16_t>(ScaleTemp(fs_temp_hb_x))
        .AddData<int16_t>(ScaleTemp(fs_temp_hb_y))
        .AddData<int16_t>(ScaleTemp(fs_temp_hb_z))
        .AddData<int16_t>(fs_radiation_1)
        .AddData<int16_t>(ScaleTemp(cdh_sys_temp))
        .AddData<int16_t>(comms_bytes_received)
        .AddData<int16_t>(comms_bytes_transmitted)
        .AddData<int16_t>(comms_antenna_flags)
        .AddData<int16_t>(comms_lithium_op_count)
        .AddData<int16_t>(comms_lithium_rssi)
        .AddData<int16_t>(comms_lithium_temp)
        .AddData<uint64_t>(comms_lithium_time.timestamp_millis_unix_epoch)
        .AddData<int16_t>(fs_angular_velocity_1)
        .AddData<int16_t>(fs_angular_velocity_2)
        .AddData<int16_t>(fs_angular_velocity_3)
        .AddData<int16_t>(fs_attitude_cov)
        .AddData<int16_t>(fs_attitude_distance)
        .AddData<int16_t>(fs_attitude_quaternion_1)
        .AddData<int16_t>(fs_attitude_quaternion_2)
        .AddData<int16_t>(fs_attitude_quaternion_3)
        .AddData<int16_t>(fs_attitude_quaternion_4)
        .AddData<int16_t>(fs_control_1)
        .AddData<int16_t>(fs_control_2)
        .AddData<int16_t>(fs_control_3)
        .AddData<uint16_t>(fs_health_flags)
        .AddData<uint16_t>(fs_attitude_flags)
        .AddData<uint16_t>(fs_flags)
        .AddData<uint16_t>(fs_location)
        .AddData<int16_t>(fs_ir)
        .AddData<uint64_t>(cdh_last_reboot.timestamp_millis_unix_epoch)
        .AddData<uint16_t>(cdh_memory)
        .AddData<uint16_t>(cdh_memory_available)
        .AddData<uint16_t>(cdh_mcu_1)
        .AddData<uint16_t>(mcu_reset_count_1);

    for (uint8_t i = 0; i < kOutreachMessageSize; i++) {
        builder.AddData<char>(comms_outreach[i]);
    }

    builder.PadZeroes();
    return builder.Build();
}

uint16_t BeaconPayload::GetSerialisedSize() const { return kPayloadSize; }

uint8_t BeaconPayload::GetPayloadCode() const { return kBeaconPayloadCode; }

int16_t BeaconPayload::ScaleArbitraryInt16(float data, uint16_t abs_max) {
    data = ConstrainToRange(data, abs_max);
    double resolution = (abs_max * 2.0) / (2 << 15);
    int32_t scaled = round(data / resolution);
    constexpr int32_t max = (2 << 14) - 1;
    return scaled > max ? max : scaled;
}

int16_t BeaconPayload::ScaleCurrent(float data) {
    return ScaleArbitraryInt16(data, kVoltageUpperBound);
}

int16_t BeaconPayload::ScaleVoltage(float data) {
    return ScaleArbitraryInt16(data, kCurrentUpperBound);
}

int16_t BeaconPayload::ScaleTemp(float data) {
    return ScaleArbitraryInt16(data, kTempUpperBound);
}

float BeaconPayload::ConstrainToRange(float data, uint16_t abs_max) {
    if (abs(data) > abs_max) {
        int8_t signum = data > abs_max ? 1 : -1;
        data = signum * abs_max;
    }
    return data;
}

double BeaconPayload::ReadCachedDouble(uint16_t measurable_id) {
    return ReadCachedMeasurable<double>(measurable_id);
}
