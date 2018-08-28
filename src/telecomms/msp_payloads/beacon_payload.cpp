#include <math.h>
#include <src/messages/serialised_message.h>
#include <src/messages/serialised_message_builder.h>
#include <src/telecomms/msp_payloads/beacon_payload.h>
#include <src/util/data_types.h>
#include <src/util/message_codes.h>

BeaconPayload::BeaconPayload()
    : com_out_i1(0),
      com_out_v1(0),
      com_out_i2(0),
      com_out_v2(0),
      com_t2(0),
      eps_adc_bat_v1(0),
      eps_load_i1(0),
      eps_adc_bat_v2(0),
      eps_boost_in_i2(0),
      eps_rail1(0),
      eps_rail2(0),
      eps_top_panel_v(0),
      eps_top_panel_i(0),
      eps_t1(0),
      eps_t2(0),
      x_pos_v(0),
      x_pos_i(0),
      x_pos_t1(0),
      x_pos_rad(0),
      y_pos_v(0),
      y_pos_i(0),
      y_pos_t1(0),
      x_neg_v(0),
      x_neg_i(0),
      x_neg_t1(0),
      y_neg_v(0),
      y_neg_i(0),
      y_neg_t1(0),
      z_neg_v(0),
      z_neg_i(0),
      z_pos_t(0),
      util_ntc_v1(0),
      util_ntc_v2(0),
      util_heat_v(0),
      util_t(0),
      fs_torquer_xi(0),
      fs_torquer_yi(0),
      fs_torquer_zi(0),
      fs_hb_xt(0),
      fs_hb_yt(0),
      fs_hb_zt(0),
      fs_rad1(0),
      cdh_t(0),
      com_rx_bytes(0),
      com_tx_bytes(0),
      com_antenna_flags(0),
      com_lithium_ops(0),
      com_lithium_rssi(0),
      com_lithium_t(0),
      com_lithium_time({0, false}),
      sw_fs_angular_velocity1(0),
      sw_fs_angular_velocity2(0),
      sw_fs_angular_velocity3(0),
      sw_fs_attitude_cov(0),
      sw_fs_attitude_distance(0),
      sw_fs_attitude_quaternion1(0),
      sw_fs_attitude_quaternion2(0),
      sw_fs_attitude_quaternion3(0),
      sw_fs_attitude_quaternion4(0),
      sw_fs_control1(0),
      sw_fs_control2(0),
      sw_fs_control3(0),
      sw_fs_health_flags(0),
      sw_fs_attitude_flags(0),
      sw_fs_flags(0),
      sw_fs_location(0),
      sw_fs_ir(0),
      sw_cdh_last_reboot({0, false}),
      sw_cdh_memory(0),
      sw_cdh_memory_available(0),
      sw_cdh_mcu1(0),
      sw_mcu_reset_count1(0),
      comms_outreach("Hello from Melbourne       ") {}
SerialisedMessage BeaconPayload::SerialiseTo(byte* serial_buffer) const {
    SerialisedMessageBuilder builder(serial_buffer, GetSerialisedSize());

    builder.AddData<int16_t>(ScaleCurrent(com_out_i1))
        .AddData<int16_t>(ScaleVoltage(com_out_v1))
        .AddData<int16_t>(ScaleCurrent(com_out_i2))
        .AddData<int16_t>(ScaleVoltage(com_out_v2))
        .AddData<int16_t>(ScaleTemp(com_t2))
        .AddData<int16_t>(ScaleVoltage(eps_adc_bat_v1))
        .AddData<int16_t>(ScaleCurrent(eps_load_i1))
        .AddData<int16_t>(ScaleVoltage(eps_adc_bat_v2))
        .AddData<int16_t>(ScaleCurrent(eps_boost_in_i2))
        .AddData<int16_t>(ScaleVoltage(eps_rail1))
        .AddData<int16_t>(ScaleVoltage(eps_rail2))
        .AddData<int16_t>(ScaleVoltage(eps_top_panel_v))
        .AddData<int16_t>(ScaleCurrent(eps_top_panel_i))
        .AddData<int16_t>(ScaleTemp(eps_t1))
        .AddData<int16_t>(ScaleTemp(eps_t2))
        .AddData<int16_t>(ScaleVoltage(x_pos_v))
        .AddData<int16_t>(ScaleCurrent(x_pos_i))
        .AddData<int16_t>(ScaleTemp(x_pos_t1))
        .AddData<int16_t>(x_pos_rad)
        .AddData<int16_t>(ScaleVoltage(y_pos_v))
        .AddData<int16_t>(ScaleCurrent(y_pos_i))
        .AddData<int16_t>(ScaleTemp(y_pos_t1))
        .AddData<int16_t>(ScaleVoltage(x_neg_v))
        .AddData<int16_t>(ScaleCurrent(x_neg_i))
        .AddData<int16_t>(ScaleTemp(x_neg_t1))
        .AddData<int16_t>(ScaleVoltage(y_neg_v))
        .AddData<int16_t>(ScaleCurrent(y_neg_i))
        .AddData<int16_t>(ScaleTemp(y_neg_t1))
        .AddData<int16_t>(ScaleVoltage(z_neg_v))
        .AddData<int16_t>(ScaleCurrent(z_neg_i))
        .AddData<int16_t>(ScaleTemp(z_neg_t1))
        .AddData<int16_t>(ScaleTemp(z_pos_t))
        .AddData<int16_t>(util_ntc_v1)
        .AddData<int16_t>(util_ntc_v2)
        .AddData<int16_t>(util_heat_v)
        .AddData<int16_t>(ScaleTemp(util_t))
        .AddData<int16_t>(ScaleCurrent(fs_torquer_xi))
        .AddData<int16_t>(ScaleCurrent(fs_torquer_yi))
        .AddData<int16_t>(ScaleCurrent(fs_torquer_zi))
        .AddData<int16_t>(ScaleTemp(fs_hb_xt))
        .AddData<int16_t>(ScaleTemp(fs_hb_yt))
        .AddData<int16_t>(ScaleTemp(fs_hb_zt))
        .AddData<uint16_t>(fs_rad1)
        .AddData<uint16_t>(cdh_t)
        .AddData<uint32_t>(com_rx_bytes)
        .AddData<uint32_t>(com_tx_bytes)
        .AddData<uint8_t>(com_antenna_flags)
        .AddData<uint16_t>(com_lithium_ops)
        .AddData<uint8_t>(com_lithium_rssi)
        .AddData<int16_t>(ScaleTemp(com_lithium_t))
        .AddData<uint64_t>(com_lithium_time.timestamp_millis_unix_epoch)
        .AddData<int16_t>(sw_fs_angular_velocity1)
        .AddData<int16_t>(sw_fs_angular_velocity2)
        .AddData<int16_t>(sw_fs_angular_velocity3)
        .AddData<int16_t>(sw_fs_attitude_cov)
        .AddData<int16_t>(sw_fs_attitude_distance)
        .AddData<int16_t>(sw_fs_attitude_quaternion1)
        .AddData<int16_t>(sw_fs_attitude_quaternion2)
        .AddData<int16_t>(sw_fs_attitude_quaternion3)
        .AddData<int16_t>(sw_fs_attitude_quaternion4)
        .AddData<int16_t>(sw_fs_control1)
        .AddData<int16_t>(sw_fs_control2)
        .AddData<int16_t>(sw_fs_control3)
        .AddData<uint16_t>(sw_fs_health_flags)
        .AddData<uint16_t>(sw_fs_attitude_flags)
        .AddData<uint16_t>(sw_fs_flags)
        .AddData<uint16_t>(sw_fs_location)
        .AddData<int16_t>(sw_fs_ir)
        .AddData<uint64_t>(sw_cdh_last_reboot.timestamp_millis_unix_epoch)
        .AddData<uint16_t>(sw_cdh_memory)
        .AddData<uint16_t>(sw_cdh_memory_available)
        .AddData<uint16_t>(sw_cdh_mcu1)
        .AddData<uint16_t>(sw_mcu_reset_count1);

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
