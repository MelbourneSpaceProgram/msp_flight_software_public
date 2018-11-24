#include <math.h>
#include <src/messages/CurrentReading.pb.h>
#include <src/messages/TemperatureReading.pb.h>
#include <src/messages/VoltageReading.pb.h>
#include <src/messages/serialised_message.h>
#include <src/messages/serialised_message_builder.h>
#include <src/payload_processor/runnable_payload_processor.h>
#include <src/telecomms/msp_payloads/beacon_payload.h>
#include <src/util/data_types.h>
#include <src/util/message_codes.h>
#include <src/util/satellite_time_source.h>
#include <stdint.h>
#include <ti/sysbios/utils/Load.h>
#include <numeric>

BeaconPayload::BeaconPayload()
    : com_out_i1(ReadCachedCurrent(kComOutI1)),
      com_out_v1(ReadCachedVoltage(kComOutV1)),
      com_out_i2(ReadCachedCurrent(kComOutI2)),
      com_out_v2(ReadCachedVoltage(kComOutV2)),
      com_t2(ReadCachedTemperature(kComT2)),
      com_antenna_status(Antenna::GetInstance()->GetStatus),

      eps_adc_bat_v1(ReadCachedVoltage(kEpsAdcBatV1)),
      eps_load_i1(ReadCachedCurrent(kEpsLoadI1)),
      eps_adc_bat_v2(ReadCachedVoltage(kEpsAdcBatV2)),
      eps_boost_in_i2(ReadCachedCurrent(kEpsBoostInI2)),
      eps_rail1(ReadCachedVoltage(kEpsRail1)),
      eps_rail2(ReadCachedVoltage(kEpsRail2)),
      eps_top_panel_v(ReadCachedVoltage(kEpsTopPanelV)),
      eps_top_panel_i(ReadCachedCurrent(kEpsTopPanelI)),
      eps_t1(ReadCachedTemperature(kEpsT1)),
      eps_t2(ReadCachedTemperature(kEpsT2)),

      x_pos_v(ReadCachedVoltage(kXPosV)),
      x_pos_i(ReadCachedCurrent(kXPosI)),
      x_pos_t1(ReadCachedTemperature(kXPosT1)),
      y_pos_v(ReadCachedVoltage(kYPosV)),
      y_pos_i(ReadCachedCurrent(kYPosI)),
      y_pos_t1(ReadCachedTemperature(kYPosT1)),
      x_neg_v(ReadCachedVoltage(kXNegV)),
      x_neg_i(ReadCachedCurrent(kXNegI)),
      x_neg_t1(ReadCachedTemperature(kXNegT1)),
      y_neg_v(ReadCachedVoltage(kYNegV)),
      y_neg_i(ReadCachedCurrent(kYNegI)),
      y_neg_t1(ReadCachedTemperature(kYNegT1)),
      z_neg_v(ReadCachedVoltage(kZNegV)),
      z_neg_i(ReadCachedCurrent(kZNegI)),
      z_neg_t1(ReadCachedTemperature(kZNegT1)),
      z_pos_t(ReadCachedTemperature(kZPosT)),

      util_t(ReadCachedTemperature(kUtilT)),

      fs_torquer_xi(ReadCachedCurrent(kFsTorquerXI)),
      fs_torquer_yi(ReadCachedCurrent(kFsTorquerYI)),
      fs_torquer_zi(ReadCachedCurrent(kFsTorquerZI)),
      fs_hb_xt(ReadCachedTemperature(kFsHbXT)),
      fs_hb_yt(ReadCachedTemperature(kFsHbYT)),
      fs_hb_zt(ReadCachedTemperature(kFsHbZT)),
      fs_rad1(kInvalidPositiveInteger),
      fs_angular_velocity1(ReadCachedMeasurable<GyroscopeReading>(kFsImuGyro1).x),
      fs_angular_velocity2(ReadCachedMeasurable<GyroscopeReading>(kFsImuGyro1).y),
      fs_angular_velocity3(ReadCachedMeasurable<GyroscopeReading>(kFsImuGyro1).z),
	  fs_magno1(ReadCachedMeasurable<MagnetometerReading>(kFsImuMagno1).x),
	  fs_magno2(ReadCachedMeasurable<MagnetometerReading>(kFsImuMagno1).y),
	  fs_magno3(ReadCachedMeasurable<MagnetometerReading>(kFsImuMagno1).z),
      fs_attitude_cov(kInvalidNegativeInteger),
      fs_attitude_quaternion1(kInvalidNegativeInteger),
      fs_attitude_quaternion2(kInvalidNegativeInteger),
      fs_attitude_quaternion3(kInvalidNegativeInteger),
      fs_attitude_quaternion4(kInvalidNegativeInteger),
      fs_flags(0),

      cdh_time(SatelliteTimeSource::GetTime()),
      cdh_t(ReadCachedTemperature(kCdhT)),

      sw_sequence(RunnablePayloadProcessor::GetSequence()),
      sw_last_reboot(SatelliteTimeSource::GetInitialTime()),
      sw_num_resets(ResetInfoContainer::GetInstance()->GetNumResets()),
      last_reset_cause(
          (static_cast<uint32_t>(
              ResetInfoContainer::GetInstance()->GetMostRecentResetCause())) %
          UINT8_MAX),
      last_reset_message(static_cast<uint8_t>(
          ResetInfoContainer::GetInstance()->GetMostRecentResetCause())),
      num_exceptions(
          std::accumulate(MspException::GetNumAll(),
                          MspException::GetNumAll() + kNumExceptionTypes, 0)),
	  sw_flags(0)
      sw_outreach("Hello from Melbourne     ") {
    // TODO(dingbenjamin): Make this a measurable and pack it smaller
    LithiumTelemetry telemetry;
    if (!Lithium::GetInstance()->DoTelemetryQuery(telemetry)) {
        telemetry = {0}:
    }
    com_lithium_rssi = telemetry.rssi;
    com_lithium_t = ScaleTemp(telemetry.msp430_temp);

    // TODO(dingbenjamin): Make this a measurable and pack it smaller
    Load_Stat stat;
    Task_Handle idlTskHandle = Task_getIdleTaskHandle(0);
    Load_getTaskLoad(idlTskHandle, &stat);
    sw_idle_load = static_cast<uint8_t>(Load_calculateLoad(&stat));
    sw_cpu_load = static_cast<uint8_t>(Load_getCPULoad());
}

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
        .AddData<int16_t>(ScaleTemp(util_t))
        .AddData<int16_t>(ScaleCurrent(fs_torquer_xi))
        .AddData<int16_t>(ScaleCurrent(fs_torquer_yi))
        .AddData<int16_t>(ScaleCurrent(fs_torquer_zi))
        .AddData<int16_t>(ScaleTemp(fs_hb_xt))
        .AddData<int16_t>(ScaleTemp(fs_hb_yt))
        .AddData<int16_t>(ScaleTemp(fs_hb_zt))
        .AddData<uint16_t>(fs_rad1)
        .AddData<uint64_t>(cdh_time.timestamp_ms)
        .AddData<uint16_t>(cdh_t)
        .AddData<uint32_t>(com_rx_bytes)
        .AddData<uint32_t>(com_tx_bytes)
        .AddData<byte>(com_antenna_status.Serialise().GetBuffer[0])
        .AddData<byte>(com_antenna_status.Serialise().GetBuffer[1])
        .AddData<byte>(com_antenna_status.Serialise().GetBuffer[2])
        .AddData<uint8_t>(com_lithium_rssi)
        .AddData<int16_t>(ScaleTemp(com_lithium_t))
        .AddData<uint64_t>(com_lithium_time.timestamp_ms)
        .AddData<int16_t>(fs_angular_velocity1)
        .AddData<int16_t>(fs_angular_velocity2)
        .AddData<int16_t>(fs_angular_velocity3)
        .AddData<int16_t>(fs_attitude_cov)
        .AddData<int16_t>(fs_attitude_distance)
        .AddData<int16_t>(fs_attitude_quaternion1)
        .AddData<int16_t>(fs_attitude_quaternion2)
        .AddData<int16_t>(fs_attitude_quaternion3)
        .AddData<int16_t>(fs_attitude_quaternion4)
        .AddData<int16_t>(fs_control1)
        .AddData<int16_t>(fs_control2)
        .AddData<int16_t>(fs_control3)
        .AddData<uint16_t>(fs_health_flags)
        .AddData<uint16_t>(fs_attitude_flags)
        .AddData<uint16_t>(fs_flags)
        .AddData<uint16_t>(fs_location)
        .AddData<int16_t>(fs_ir)
        .AddData<uint64_t>(sw_last_reboot.timestamp_ms)
        .AddData<uint16_t>(sw_sequence);
		.AddData<uint8_t>(sw_idle_load);
		.AddData<uint8_t>(sw_cpu_load);
        .AddData<uint16_t>(num_resets)
        .AddData<byte>(last_reset_cause)
        .AddData<byte>(last_reset_message)
        .AddData<uint16_t>(num_exceptions)

    for (uint8_t i = 0; i < kOutreachMessageSize; i++) {
        builder.AddData<char>(comms_outreach[i]);
    }

    builder.PadZeroes();
    return builder.Build();
}

uint16_t BeaconPayload::GetSerialisedSize() const { return kPayloadSize; }

uint8_t BeaconPayload::GetPayloadCode() const { return kBeaconPayloadCode; }

int16_t BeaconPayload::ScaleArbitraryInt16(float data, uint16_t abs_max,
                                           float invalid_value) {
    if (fabs(data - invalid_value) <= std::numeric_limits<float>::epsilon()) {
        // This flag is used on the ground station to signify an 'invalid' value
        // which is different to the minimum value
        return kInvalidScaled16BitValue;
    }

    data = ConstrainToRange(data, abs_max);
    double resolution = (abs_max * 2.0) / (2 << 15);
    int32_t scaled = round(data / resolution);
    constexpr int32_t max = (2 << 14) - 1;
    if (scaled > max) return max;
    if (scaled < -max) return -max;
    return scaled;
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

int16_t BeaconPayload::ScaleGyro(float data) {
    return ScaleArbitraryInt16(data, kGyroUpperBound);
}

int16_t BeaconPayload::ScaleMagno(float data) {
    return ScaleArbitraryInt16(data, kMagnoUpperBound);
}

double BeaconPayload::ReadCachedTemperature(uint16_t measurable_id) {
    return ReadCachedMeasurable<TemperatureReading>(measurable_id).temp;
}

double BeaconPayload::ReadCachedVoltage(uint16_t measurable_id) {
    return ReadCachedMeasurable<VoltageReading>(measurable_id).voltage;
}

double BeaconPayload::ReadCachedCurrent(uint16_t measurable_id) {
    return ReadCachedMeasurable<CurrentReading>(measurable_id).current;
}

float BeaconPayload::ConstrainToRange(float data, uint16_t abs_max) {
    if (fabs(data) > abs_max) {
        int8_t signum = data > abs_max ? 1 : -1;
        data = signum * abs_max;
    }
    return data;
}
