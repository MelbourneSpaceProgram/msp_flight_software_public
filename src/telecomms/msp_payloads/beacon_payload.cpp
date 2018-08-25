#include <src/messages/serialised_message.h>
#include <src/messages/serialised_message_builder.h>
#include <src/telecomms/msp_payloads/beacon_payload.h>
#include <src/util/message_codes.h>

BeaconPayload::BeaconPayload()
    : comms_regulator_1_output_current(0),
      comms_regulator_1_output_voltage(0),
      comms_regulator_2_output_current(0),
      comms_regulator_2_output_voltage(0),
      comms_temp_2(0),
      power_bat_voltage_1(0),
      power_load_current_1(0),
      power_bat_voltage_2(0),
      power_load_current_2(0),
      power_5v_rail_1(0),
      power_5v_rail_2(0),
      power_top_panel_voltage(0),
      power_top_panel_current(0),
      power_temp_1(0),
      power_temp_2(0),
      power_panel_voltage_access_x_axis(0),
      power_panel_current_access_x_axis(0),
      power_solar_voltage_access_x_axis(0),
      power_solar_current_access_x_axis(0),
      power_access_panel_x_axis_temp_1(0),
      power_access_panel_x_axis_temp_2(0),
      fs_radiation_access_panel_x_axis(0),
      power_panel_voltage_side_1_y_axis(0),
      power_panel_current_side_1_y_axis(0),
      power_solar_voltage_side_1_y_axis(0),
      power_solar_current_side_1_y_axis(0),
      power_side_1_panel_temp_1_y_axis(0),
      power_side_1_panel_y_axis_temp_2(0),
      power_panel_voltage_side_2_minus_x_axis(0),
      power_panel_current_side_2_minus_x_axis(0),
      power_solar_voltage_side_2_minus_x_axis(0),
      power_solar_current_side_2_minus_x_axis(0),
      power_side_2_panel_minus_x_axis_temp_1(0),
      power_side_2_panel_minus_x_axis_temp_2(0),
      power_panel_voltage_side_3_minus_y_axis(0),
      power_panel_current_side_3_minus_y_axis(0),
      power_solar_voltage_side_3_minus_y_axis(0),
      power_solar_current_side_3_minus_y_axis(0),
      power_side_3_panel_minus_y_axis_temp_1(0),
      power_side_3_panel_minus_y_axis_temp_2(0),
      power_panel_voltage_bottom_minus_z_axis(0),
      power_panel_current_bottom_minus_z_axis(0),
      power_solar_voltage_bottom_minus_z_axis(0),
      power_solar_current_bottom_minus_z_axis(0),
      power_bottom_panel_minus_z_axis_temp_1(0),
      power_bottom_panel_minus_z_axis_temp_2(0),
      power_top_panel_z_axis_temp_1(0),
      util_ntc_1(0),
      util_ntc_2(0),
      util_heating(0),
      util_temp(0),
      fs_torquer_current_x(0),
      fs_torquer_current_y(0),
      fs_torquer_current_z(0),
      fs_temp_hb_x(0),
      fs_temp_hb_y(0),
      fs_temp_hb_z(0),
      fs_radiation_1(0),
      cdh_sys_temp(0),
      comms_bytes_received(0),
      comms_bytes_transmitted(0),
      comms_antenna_flags(0),
      comms_lithium_op_count(0),
      comms_lithium_rssi(0),
      comms_lithium_temp(0),
      comms_lithium_time(),
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
      cdh_last_reboot(0),
      comms_outreach("Hello from Melbourne       ") {
    fs_attitude_flags.fill(0);
    fs_health_flags.fill(0);
    fs_location.fill(0);
}

SerialisedMessage BeaconPayload::SerialiseTo(byte* serial_buffer) const {
    SerialisedMessageBuilder builder(serial_buffer, GetSerialisedSize());

    builder.AddData<float>(fs_attitude_quaternion_1)
        .AddData<float>(fs_attitude_quaternion_2)
        .AddData<float>(fs_attitude_quaternion_3)
        .AddData<float>(fs_attitude_quaternion_4)
        .AddData<uint16_t>(fs_angular_velocity_1)
        .AddData<uint16_t>(fs_angular_velocity_2)
        .AddData<uint16_t>(fs_angular_velocity_3)
        .AddData<uint8_t>(fs_attitude_cov)
        .AddData<uint8_t>(fs_attitude_distance)
        .AddData<float>(comms_regulator_1_output_current)
        .AddData<float>(comms_regulator_1_output_voltage)
        .AddData<float>(comms_regulator_2_output_current)
        .AddData<float>(comms_regulator_2_output_voltage)
        .AddData<uint8_t>(comms_temp_2)
        .AddData<uint8_t>(power_bat_voltage_1)
        .AddData<uint8_t>(power_load_current_1)
        .AddData<float>(power_bat_voltage_2)
        .AddData<float>(power_load_current_2)
        .AddData<float>(power_5v_rail_1)
        .AddData<uint16_t>(power_5v_rail_2)
        .AddData<uint16_t>(power_top_panel_voltage)
        .AddData<uint8_t>(power_top_panel_voltage)
        .AddData<uint8_t>(power_top_panel_current)
        .AddData<uint16_t>(power_temp_2)
        .AddData<uint16_t>(power_panel_voltage_access_x_axis)
        .AddData<byte>(power_panel_current_access_x_axis)
        .AddData<byte>(power_solar_voltage_access_x_axis)
        .AddData<byte>(power_solar_current_access_x_axis)
        .AddData<byte>(power_access_panel_x_axis_temp_1)
        .AddData<byte>(power_access_panel_x_axis_temp_2)
        .AddData<byte>(fs_radiation_access_panel_x_axis)
        .AddData<byte>(power_panel_voltage_side_1_y_axis)
        .AddData<byte>(power_panel_current_side_1_y_axis)
        .AddData<byte>(power_solar_voltage_side_1_y_axis)
        .AddData<byte>(power_solar_current_side_1_y_axis)
        .AddData<byte>(power_side_1_panel_y_axis_temp_1)
        .AddData<uint16_t>(power_side_1_panel_y_axis_temp_2)
        .AddData<byte>(power_panel_voltage_side_2_minus_x_axis)
        .AddData<uint16_t>(power_panel_current_side_2_minus_x_axis)
        .AddData<byte>(power_solar_voltage_side_2_minus_x_axis)
        .AddData<byte>(power_solar_current_side_2_minus_x_axis)
        .AddData<byte>(power_side_2_panel_minus_x_axis_temp_1)
        .AddData<byte>(power_side_2_panel_minus_x_axis_temp_2)
        .AddData<byte>(power_panel_voltage_side_3_minus_y_axis)
        .AddData<byte>(power_panel_current_side_3_minus_y_axis)
        .AddData<byte>(power_solar_voltage_side_3_minus_y_axis)
        .AddData<uint16_t>(power_solar_current_side_3_minus_y_axis)
        .AddData<uint16_t>(power_side_3_panel_minus_y_axis_temp_1)
        .AddData<byte>(power_side_3_panel_minus_y_axis_temp_2)
        .AddData<uint8_t>(power_panel_voltage_bottom_minus_z_axis)
        .AddData<uint32_t>(power_panel_current_bottom_minus_z_axis)
        .AddData<uint32_t>(power_solar_current_bottom_minus_z_axis)
        .AddData<byte>(power_bottom_panel_minus_z_axis_temp_1)
        .AddData<byte>(power_bottom_panel_minus_z_axis_temp_2)
        .AddData<byte>(power_top_panel_z_axis_temp_1)
        .AddData<byte>(util_ntc_1)
        .AddData<byte>(util_ntc_2)
        .AddData<byte>(util_heating)
        .AddData<byte>(util_temp)
        .AddData<byte>(fs_torquer_current_x)
        .AddData<byte>(fs_torquer_current_y)
        .AddData<byte>(fs_torquer_current_z)
        .AddData<byte>(fs_temp_hb_x)
        .AddData<byte>(fs_temp_hb_y)
        .AddData<byte>(fs_temp_hb_z)
        .AddData<byte>(fs_radiation_1)
        .AddData<byte>(cdh_sys_temp)
        .AddData<byte>(comms_bytes_received)
        .AddData<byte>(comms_bytes_transmitted)
        .AddData<byte>(comms_antenna_flags)
        .AddData<byte>(comms_lithium_op_count)
        .AddData<uint16_t>(comms_lithium_rssi)
        .AddData<uint16_t>(comms_lithium_temp)
        .AddData<uint64_t>(comms_lithium_time.timestamp_millis_unix_epoch)
        .AddData<byte>(fs_control_1)
        .AddData<byte>(fs_control_2)
        .AddData<byte>(fs_control_3)
        .AddEtlArray<byte, kFsFsAttitudeFlagByteCount>(fs_attitude_flags)
        .AddEtlArray<byte, kFsHealthFlagByteCount>(fs_health_flags)
        .AddEtlArray<float, kFsLocationFloatCount>(fs_location)
        .AddData<byte>(fs_flags)
        .AddData<uint16_t>(fs_ir)
        .AddData<uint16_t>(cdh_mcu_1)
        .AddData<uint16_t>(mcu_reset_count_1)
        .AddData<byte>(cdh_memory_available)
        .AddData<uint64_t>(cdh_last_reboot.timestamp_millis_unix_epoch);

    for (uint8_t i = 0; i < kOutreachMessageSize; i++) {
        builder.AddData<char>(comms_outreach[i]);
    }

    builder.PadZeroes();
    return builder.Build();
}

uint16_t BeaconPayload::GetSerialisedSize() const { return kPayloadSize; }

uint8_t BeaconPayload::GetPayloadCode() const { return kBeaconPayloadCode; }

int16_t BeaconPayload::ScaleCurrentMeasurable(float data) {
    ConstrainToRange(data, kCurrentUpperBound);
    data = data * kCurrentScaleFactor;
    return static_cast<int16_t>(data);
}

int16_t BeaconPayload::ScaleVoltageMeasurable(float data) {
    data = ConstrainToRange(data, kVoltageUpperBound);
    data = data * kVoltageScaleFactor;
    return static_cast<int16_t>(data);
}

byte BeaconPayload::ScaleTempMeasurable(float data) {
    data = ConstrainToRange(data, kTempUpperBound);
    data = data * kTempScaleFactor;
    return static_cast<byte>(data);
}

float BeaconPayload::ConstrainToRange(float data, uint8_t upper_bound) {
    if (abs(data) > upper_bound) {
        int8_t signum = data > upper_bound ? 1 : -1;
        data = signum * upper_bound;
    }
    return data;
}


/***********TODO(naverill) Remove getters after testing **********/
BeaconPayload* BeaconPayload::GetCommsRegulator1OutputCurrent() {
      return this->comms_regulator_1_output_current/kCurrentScaleFactor;
}

BeaconPayload* BeaconPayload::GetCommsRegulator1OutputVoltage() {
    return this->comms_regulator_1_output_voltage/kVoltageScaleFactor;
}

BeaconPayload* BeaconPayload::GetCommsTemp2() {
    return this->comms_temp_2/kTempScaleFactor;
}

/******************************************************************/

BeaconPayload* BeaconPayload::SetCommsRegulator1OutputCurrent(
    float comms_regulator_1_output_current) {
    this->comms_regulator_1_output_current =
        ScaleCurrentMeasurable(comms_regulator_1_output_current);
    return this;
}

BeaconPayload* BeaconPayload::SetCommsRegulator1OutputVoltage(
    float comms_regulator_1_output_voltage) {
    this->comms_regulator_1_output_voltage =
        ScaleCurrentMeasurable(comms_regulator_1_output_voltage);
    return this;
}

BeaconPayload* BeaconPayload::SetCommsRegulator2OutputCurrent(
    float comms_regulator_2_output_current) {
    this->comms_regulator_2_output_current =
        ScaleCurrentMeasurable(comms_regulator_2_output_current);
    return this;
}

BeaconPayload* BeaconPayload::SetCommsRegulator2OutputVoltage(
    float comms_regulator_2_output_voltage) {
    this->comms_regulator_2_output_voltage =
        ScaleVoltageMeasurable(comms_regulator_2_output_voltage);
    return this;
}

BeaconPayload* BeaconPayload::SetCommsTemp2(byte comms_temp_2) {
    this->comms_temp_2 = ScaleTempMeasurable(comms_temp_2);
    return this;
}

BeaconPayload* BeaconPayload::SetPowerBatVoltage1(float power_bat_voltage_1) {
    this->power_bat_voltage_1 = ScaleVoltageMeasurable(power_bat_voltage_1);
    return this;
}

BeaconPayload* BeaconPayload::SetPowerLoadCurrent1(float power_load_current_1) {
    this->power_load_current_1 = ScaleCurrentMeasurable(power_load_current_1);
    return this;
}

BeaconPayload* BeaconPayload::SetPowerBatVoltage2(float power_bat_voltage_2) {
    this->power_bat_voltage_2 = ScaleVoltageMeasurable(power_bat_voltage_2);
    return this;
}

BeaconPayload* BeaconPayload::SetPowerLoadCurrent2(float power_load_current_2) {
    this->power_load_current_2 = ScaleCurrentMeasurable(power_load_current_2);
    return this;
}

BeaconPayload* BeaconPayload::SetPower5VRail1(float power_5v_rail_1) {
    this->power_5v_rail_1 = ScaleVoltageMeasurable(power_5v_rail_1);
    return this;
}

BeaconPayload* BeaconPayload::SetPower5VRail2(float power_5v_rail_2) {
    this->power_5v_rail_2 = ScaleVoltageMeasurable(power_5v_rail_2);
    return this;
}

BeaconPayload* BeaconPayload::SetPowerTopPanelVoltage(
    float power_top_panel_voltage) {
    this->power_top_panel_voltage =
        ScaleVoltageMeasurable(power_top_panel_voltage);
    return this;
}

BeaconPayload* BeaconPayload::SetPowerTopPanelCurrent(
    float power_top_panel_current) {
    this->power_top_panel_current =
        ScaleCurrentMeasurable(power_top_panel_current);
    return this;
}

BeaconPayload* BeaconPayload::SetPowerTemp1(byte power_temp_1) {
    this->power_temp_1 = ScaleTempMeasurable(power_temp_1);
    return this;
}

BeaconPayload* BeaconPayload::SetPowerTemp2(byte power_temp_2) {
    this->power_temp_2 = ScaleTempMeasurable(power_temp_2);
    return this;
}

BeaconPayload* BeaconPayload::SetPowerPanelVoltageAccessXAxis(
    float power_panel_voltage_access_x_axis) {
    this->power_panel_voltage_access_x_axis =
        ScaleVoltageMeasurable(power_panel_voltage_access_x_axis);
    return this;
}

BeaconPayload* BeaconPayload::SetPowerPanelCurrentAccessXAxis(
    float power_panel_current_access_x_axis) {
    this->power_panel_current_access_x_axis =
        ScaleCurrentMeasurable(power_panel_current_access_x_axis);
    return this;
}

BeaconPayload* BeaconPayload::SetPowerSolarVoltageAccessXAxis(
    float power_solar_voltage_access_x_axis) {
    this->power_solar_voltage_access_x_axis =
        ScaleVoltageMeasurable(power_solar_voltage_access_x_axis);
    return this;
}

BeaconPayload* BeaconPayload::SetPowerSolarCurrentAccessXAxis(
    float power_solar_current_access_x_axis) {
    this->power_solar_current_access_x_axis =
        ScaleCurrentMeasurable(power_solar_current_access_x_axis);
    return this;
}

BeaconPayload* BeaconPayload::SetPowerAccessPanelXAxisTemp1(
    byte power_access_panel_x_axis_temp_1) {
    this->power_access_panel_x_axis_temp_1 =
        ScaleTempMeasurable(power_access_panel_x_axis_temp_1);
    return this;
}

BeaconPayload* BeaconPayload::SetPowerAccessPanelXAxisTemp2(
    byte power_access_panel_x_axis_temp_2) {
    this->power_access_panel_x_axis_temp_2 =
        ScaleTempMeasurable(power_access_panel_x_axis_temp_2);
    return this;
}

BeaconPayload* BeaconPayload::SetFsRadiationAccessPanelXAxis(
    byte fs_radiation_access_panel_x_axis) {
    this->fs_radiation_access_panel_x_axis = fs_radiation_access_panel_x_axis;
    return this;
}

BeaconPayload* BeaconPayload::SetPowerPanelVoltageSide1YAxis(
    float power_panel_voltage_side_1_y_axis) {
    this->power_panel_voltage_side_1_y_axis =
        ScaleVoltageMeasurable(power_panel_voltage_side_1_y_axis);
    return this;
}

BeaconPayload* BeaconPayload::SetPowerPanelCurrentSide1YAxis(
    float power_panel_current_side_1_y_axis) {
    this->power_panel_current_side_1_y_axis =
        ScaleCurrentMeasurable(power_panel_current_side_1_y_axis);
    return this;
}

BeaconPayload* BeaconPayload::SetPowerSolarVoltageSide1YAxis(
    float power_solar_voltage_side_1_y_axis) {
    this->power_solar_voltage_side_1_y_axis =
        ScaleVoltageMeasurable(power_solar_voltage_side_1_y_axis);
    return this;
}

BeaconPayload* BeaconPayload::SetPowerSolarCurrentSide1YAxis(
    float power_solar_current_side_1_y_axis) {
    this->power_solar_current_side_1_y_axis =
        ScaleCurrentMeasurable(power_solar_current_side_1_y_axis);
    return this;
}

BeaconPayload* BeaconPayload::SetPowerSide1PanelTemp1YAxis(
    byte power_side_1_panel_y_axis_temp_1) {
    this->power_side_1_panel_y_axis_temp_1 =
        ScaleTempMeasurable(power_side_1_panel_y_axis_temp_1);
    return this;
}

BeaconPayload* BeaconPayload::SetPowerSide1PanelYAxisTemp2(
    byte power_side_1_panel_y_axis_temp_2) {
    this->power_side_1_panel_y_axis_temp_2 =
        ScaleTempMeasurable(power_side_1_panel_y_axis_temp_2);
    return this;
}

BeaconPayload* BeaconPayload::SetPowerPanelVoltageSide2MinusXAxis(
    float power_panel_voltage_side_2_minus_x_axis) {
    this->power_panel_voltage_side_2_minus_x_axis =
        ScaleVoltageMeasurable(power_panel_voltage_side_2_minus_x_axis);
    return this;
}

BeaconPayload* BeaconPayload::SetPowerPanelCurrentSide2MinusXAxis(
    float power_panel_current_side_2_minus_x_axis) {
    this->power_panel_current_side_2_minus_x_axis =
        ScaleCurrentMeasurable(power_panel_current_side_2_minus_x_axis);
    return this;
}

BeaconPayload* BeaconPayload::SetPowerSolarVoltageSide2MinusXAxis(
    float power_solar_voltage_side_2_minus_x_axis) {
    this->power_solar_voltage_side_2_minus_x_axis =
        ScaleVoltageMeasurable(power_solar_voltage_side_2_minus_x_axis);
    return this;
}

BeaconPayload* BeaconPayload::SetPowerSolarCurrentSide2MinusXAxis(
    float power_solar_current_side_2_minus_x_axis) {
    this->power_solar_current_side_2_minus_x_axis =
        ScaleCurrentMeasurable(power_solar_current_side_2_minus_x_axis);
    return this;
}

BeaconPayload* BeaconPayload::SetPowerSide2PanelMinusXAxisTemp1(
    byte power_side_2_panel_minus_x_axis_temp_1) {
    this->power_side_2_panel_minus_x_axis_temp_1 =
        ScaleTempMeasurable(power_side_2_panel_minus_x_axis_temp_1);
    return this;
}

BeaconPayload* BeaconPayload::SetPowerSide2PanelMinusXAxisTemp2(
    byte power_side_2_panel_minus_x_axis_temp_2) {
    this->power_side_2_panel_minus_x_axis_temp_2 =
        ScaleTempMeasurable(power_side_2_panel_minus_x_axis_temp_2);
    return this;
}

BeaconPayload* BeaconPayload::SetPowerPanelVoltageSide3MinusYAxis(
    float power_panel_voltage_side_3_minus_y_axis) {
    this->power_panel_voltage_side_3_minus_y_axis =
        power_panel_voltage_side_3_minus_y_axis;
    return this;
}

BeaconPayload* BeaconPayload::SetPowerPanelCurrentSide3MinusYAxis(
    float power_panel_current_side_3_minus_y_axis) {
    this->power_panel_current_side_3_minus_y_axis =
        ScaleCurrentMeasurable(power_panel_current_side_3_minus_y_axis);
    return this;
}

BeaconPayload* BeaconPayload::SetPowerSolarVoltageSide3MinusYAxis(
    float power_solar_voltage_side_3_minus_y_axis) {
    this->power_solar_voltage_side_3_minus_y_axis =
        ScaleVoltageMeasurable(power_solar_voltage_side_3_minus_y_axis);
    return this;
}

BeaconPayload* BeaconPayload::SetPowerSolarCurrentSide3MinusYAxis(
    float power_solar_current_side_3_minus_y_axis) {
    this->power_solar_current_side_3_minus_y_axis =
        ScaleCurrentMeasurable(power_solar_current_side_3_minus_y_axis);
    return this;
}

BeaconPayload* BeaconPayload::SetPowerSide3PanelMinusYAxisTemp1(
    byte power_side_3_panel_minus_y_axis_temp_1) {
    this->power_side_3_panel_minus_y_axis_temp_1 =
        ScaleTempMeasurable(power_side_3_panel_minus_y_axis_temp_1);
    return this;
}

BeaconPayload* BeaconPayload::SetPowerSide3PanelMinusYAxisTemp2(
    byte power_side_3_panel_minus_y_axis_temp_2) {
    this->power_side_3_panel_minus_y_axis_temp_2 =
        ScaleTempMeasurable(power_side_3_panel_minus_y_axis_temp_2);
    return this;
}

BeaconPayload* BeaconPayload::SetPowerPanelVoltageBottomMinusZAxis(
    float power_panel_voltage_bottom_minus_z_axis) {
    this->power_panel_voltage_bottom_minus_z_axis =
        ScaleVoltageMeasurable(power_panel_voltage_bottom_minus_z_axis);
    return this;
}

BeaconPayload* BeaconPayload::SetPowerPanelCurrentBottomMinusZAxis(
    float power_panel_current_bottom_minus_z_axis) {
    this->power_panel_current_bottom_minus_z_axis =
        ScaleCurrentMeasurable(power_panel_current_bottom_minus_z_axis);
    return this;
}

BeaconPayload* BeaconPayload::SetPowerSolarVoltageBottomMinusZAxis(
    float power_solar_voltage_bottom_minus_z_axis) {
    this->power_solar_voltage_bottom_minus_z_axis =
        ScaleVoltageMeasurable(power_solar_voltage_bottom_minus_z_axis);
    return this;
}

BeaconPayload* BeaconPayload::SetPowerSolarCurrentBottomMinusZAxis(
    float power_solar_current_bottom_minus_z_axis) {
    this->power_solar_current_bottom_minus_z_axis =
        ScaleCurrentMeasurable(power_solar_current_bottom_minus_z_axis);
    return this;
}

BeaconPayload* BeaconPayload::SetPowerBottomPanelMinusZAxisTemp1(
    byte power_bottom_panel_minus_z_axis_temp_1) {
    this->power_bottom_panel_minus_z_axis_temp_1 =
        ScaleTempMeasurable(power_bottom_panel_minus_z_axis_temp_1);
    return this;
}

BeaconPayload* BeaconPayload::SetPowerBottomPanelMinusZAxisTemp2(
    byte power_bottom_panel_minus_z_axis_temp_2) {
    this->power_bottom_panel_minus_z_axis_temp_2 =
        ScaleTempMeasurable(power_bottom_panel_minus_z_axis_temp_2);
    return this;
}

BeaconPayload* BeaconPayload::SetPowerTopPanelZAxisTemp1(
    byte power_top_panel_z_axis_temp_1) {
    this->power_top_panel_z_axis_temp_1 =
        ScaleTempMeasurable(power_top_panel_z_axis_temp_1);
    return this;
}

BeaconPayload* BeaconPayload::SetUtilitiesNtc1(byte util_ntc_1) {
    this->util_ntc_1 = util_ntc_1;
    return this;
}

BeaconPayload* BeaconPayload::SetUtilitiesNtc2(uint16_t util_ntc_2) {
    this->util_ntc_2 = util_ntc_2;
    return this;
}

BeaconPayload* BeaconPayload::SetUtilitiesHeating(uint16_t util_heating) {
    this->util_heating = util_heating;
    return this;
}

BeaconPayload* BeaconPayload::SetUtilitiesTemp(byte util_temp) {
    this->util_temp = ScaleTempMeasurable(util_temp);
    return this;
}

BeaconPayload* BeaconPayload::SetFsTorquerCurrentX(float fs_torquer_current_x) {
    this->fs_torquer_current_x = ScaleCurrentMeasurable(fs_torquer_current_x);
    return this;
}

BeaconPayload* BeaconPayload::SetFsTorquerCurrentY(float fs_torquer_current_y) {
    this->fs_torquer_current_y = ScaleCurrentMeasurable(fs_torquer_current_y);
    return this;
}

BeaconPayload* BeaconPayload::SetFsTorquerCurrentZ(float fs_torquer_current_z) {
    this->fs_torquer_current_z = ScaleCurrentMeasurable(fs_torquer_current_z);
    return this;
}

BeaconPayload* BeaconPayload::SetFsTempHbX(byte fs_temp_hb_x) {
    this->fs_temp_hb_x = ScaleTempMeasurable(fs_temp_hb_x);
    return this;
}

BeaconPayload* BeaconPayload::SetFsTempHbY(byte fs_temp_hb_y) {
    this->fs_temp_hb_y = ScaleTempMeasurable(fs_temp_hb_y);
    return this;
}

BeaconPayload* BeaconPayload::SetFsTempHbZ(byte fs_temp_hb_z) {
    this->fs_temp_hb_z = ScaleTempMeasurable(fs_temp_hb_z);
    return this;
}

BeaconPayload* BeaconPayload::SetFsRadiation1(uint16_t fs_radiation_1) {
    this->fs_radiation_1 = fs_radiation_1;
    return this;
}

BeaconPayload* BeaconPayload::SetCdhSysTemp(byte cdh_sys_temp) {
    this->cdh_sys_temp = ScaleTempMeasurable(cdh_sys_temp);
    return this;
}

BeaconPayload* BeaconPayload::SetCommsBytesReceived(
    uint16_t comms_bytes_received) {
    this->comms_bytes_received = comms_bytes_received;
    return this;
}

BeaconPayload* BeaconPayload::SetCommsBytesTransmitted(
    uint16_t comms_bytes_transmitted) {
    this->comms_bytes_transmitted = comms_bytes_transmitted;
    return this;
}

BeaconPayload* BeaconPayload::SetCommsAntennaFlags(uint16_t comms_antenna_flags) {
    this->comms_antenna_flags = comms_antenna_flags;
    return this;
}

BeaconPayload* BeaconPayload::SetCommsLithiumOpCount(byte comms_lithium_op_count) {
    this->comms_lithium_op_count = comms_lithium_op_count;
    return this;
}

BeaconPayload* BeaconPayload::SetCommsLithiumRssi(uint16_t comms_lithium_rssi) {
    this->comms_lithium_rssi = comms_lithium_rssi;
    return this;
}

BeaconPayload* BeaconPayload::SetCommsLithiumTemp(uint16_t comms_lithium_temp) {
    this->comms_lithium_temp = ScaleTempMeasurable(comms_lithium_temp);
    return this;
}

BeaconPayload* BeaconPayload::SetCommsLithiumTime(Time comms_lithium_time) {
    this->comms_lithium_time = comms_lithium_time;
    return this;
}

BeaconPayload* BeaconPayload::SetFsIr(uint8_t fs_ir) {
    this->fs_ir = fs_ir;
    return this;
}

BeaconPayload* BeaconPayload::SetFsAngularVelocity1(float fs_angular_velocity_1) {
    this->fs_angular_velocity_1 = fs_angular_velocity_1;
    return this;
}

BeaconPayload* BeaconPayload::SetFsAngularVelocity2(float fs_angular_velocity_2) {
    this->fs_angular_velocity_2 = fs_angular_velocity_2;
    return this;
}

BeaconPayload* BeaconPayload::SetFsAngularVelocity3(float fs_angular_velocity_3) {
    this->fs_angular_velocity_3 = fs_angular_velocity_3;
    return this;
}

BeaconPayload* BeaconPayload::SetFsAttitudeCov(uint8_t fs_attitude_cov) {
    this->fs_attitude_cov = fs_attitude_cov;
    return this;
}

BeaconPayload* BeaconPayload::SetFsAttitudeDistance(
    uint8_t fs_attitude_distance) {
    this->fs_attitude_distance = fs_attitude_distance;
    return this;
}

BeaconPayload* BeaconPayload::SetFsAttitudeFlags(
    etl::array<byte, kFsFsAttitudeFlagByteCount> fs_attitude_flags) {
    this->fs_attitude_flags = fs_attitude_flags;
    return this;
}

BeaconPayload* BeaconPayload::SetFsAttitudeQuaternion1(
    float fs_attitude_quaternion_1) {
    this->fs_attitude_quaternion_1 = fs_attitude_quaternion_1;
    return this;
}

BeaconPayload* BeaconPayload::SetFsAttitudeQuaternion2(
    float fs_attitude_quaternion_2) {
    this->fs_attitude_quaternion_2 = fs_attitude_quaternion_2;
    return this;
}

BeaconPayload* BeaconPayload::SetFsAttitudeQuaternion3(
    float fs_attitude_quaternion_3) {
    this->fs_attitude_quaternion_3 = fs_attitude_quaternion_3;
    return this;
}

BeaconPayload* BeaconPayload::SetFsAttitudeQuaternion4(
    float fs_attitude_quaternion_4) {
    this->fs_attitude_quaternion_4 = fs_attitude_quaternion_4;
    return this;
}

BeaconPayload* BeaconPayload::SetFsControl1(uint8_t fs_control_1) {
    this->fs_control_1 = fs_control_1;
    return this;
}

BeaconPayload* BeaconPayload::SetFsControl2(uint8_t fs_control_2) {
    this->fs_control_2 = fs_control_2;
    return this;
}

BeaconPayload* BeaconPayload::SetFsControl3(uint8_t fs_control_3) {
    this->fs_control_3 = fs_control_3;
    return this;
}

BeaconPayload* BeaconPayload::SetFsFlags(byte fs_flags) {
    this->fs_flags = fs_flags;
    return this;
}

BeaconPayload* BeaconPayload::SetFsHealthFlags(
    etl::array<byte, kFsHealthFlagByteCount> fs_health_flags) {
    this->fs_health_flags = fs_health_flags;
    return this;
}

BeaconPayload* BeaconPayload::SetFsLocation(
    etl::array<float, kFsLocationFloatCount> fs_location) {
    this->fs_location = fs_location;
    return this;
}

BeaconPayload* BeaconPayload::SetMcuResetCount1(uint8_t mcu_reset_count_1) {
    this->mcu_reset_count_1 = mcu_reset_count_1;
    return this;
}

BeaconPayload* BeaconPayload::SetCdhMemory(uint16_t cdh_memory) {
    this->cdh_memory = cdh_memory;
    return this;
}

BeaconPayload* BeaconPayload::SetCdhMemoryAvailable(
    uint16_t cdh_memory_available) {
    this->cdh_memory_available = cdh_memory_available;
    return this;
}

BeaconPayload* BeaconPayload::SetCdhLastReboot(const Time& cdh_last_reboot) {
    this->cdh_last_reboot = cdh_last_reboot;
    return this;
}
