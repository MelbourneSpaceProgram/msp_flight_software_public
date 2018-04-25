#include <src/messages/serialised_message.h>
#include <src/messages/serialised_message_builder.h>
#include <src/telecomms/msp_payloads/beacon_payload.h>
#include <src/util/message_codes.h>

BeaconPayload::BeaconPayload()
    : fs_attitude_quaternion_1(0),
      fs_attitude_quaternion_2(0),
      fs_attitude_quaternion_3(0),
      fs_attitude_quaternion_4(0),
      fs_angular_velocity_1(0),
      fs_angular_velocity_2(0),
      fs_angular_velocity_3(0),
      fs_attitude_cov(0),
      fs_attitude_distance(0),
      fs_torquer_temp_1(0),
      fs_torquer_temp_2(0),
      fs_torquer_temp_3(0),
      fs_flags(0),
      fs_nadir_1(0),
      fs_nadir_2(0),
      fs_nadir_3(0),
      fs_ir(0),
      fs_control_1(0),
      fs_control_2(0),
      fs_control_3(0),
      fs_torquer_current_1(0),
      fs_torquer_current_2(0),
      fs_torquer_current_3(0),
      cdh_mcu_1(0),
      cdh_mcu_2(0),
      cdh_time(),
      mcu_reset_count_1(0),
      mcu_reset_count_2(0),
      cdh_memory(0),
      cdh_memory_available(0),
      cdh_mcu_temp_1(0),
      cdh_mcu_temp_2(0),
      cdh_reg_voltage_1(0),
      cdh_reg_current_1(0),
      cdh_reg_temp_1(0),
      cdh_reg_voltage_2(0),
      cdh_reg_current_2(0),
      cdh_reg_temp_2(0),
      cdh_reg_voltage_3(0),
      cdh_reg_current_3(0),
      cdh_reg_temp_3(0),
      cdh_bus_fault(0),
      cdh_last_reboot(),
      cdh_rtc_status(0),
      cdh_heartbeat(0),
      comms_outreach("Hello from Melbourne       "),
      comms_temp_1(0),
      comms_temp_2(0),
      comms_current_1(0),
      comms_current_2(0),
      comms_current_3(0),
      comms_voltage_1(0),
      comms_voltage_2(0),
      comms_lithium_op_count(0),
      comms_lithium_temp(0),
      comms_lithium_time(0),
      comms_lithium_rssi(0),
      comms_bytes_received(0),
      comms_bytes_transmitted(0),
      comms_antenna_flags(0),
      power_solar_voltage_1(0),
      power_solar_voltage_2(0),
      power_solar_voltage_3(0),
      power_solar_voltage_4(0),
      power_solar_voltage_5(0),
      power_solar_voltage_6(0),
      power_solar_current_1(0),
      power_solar_current_2(0),
      power_solar_current_3(0),
      power_solar_current_4(0),
      power_solar_current_5(0),
      power_solar_current_6(0),
      power_panel_current_1(0),
      power_panel_current_2(0),
      power_panel_current_3(0),
      power_panel_current_4(0),
      power_panel_current_5(0),
      power_panel_current_6(0),
      power_in_voltage_1(0),
      power_in_voltage_2(0),
      power_in_current_1(0),
      power_in_current_2(0),
      power_bat_voltage_1(0),
      power_bat_voltage_2(0),
      power_bat_current_1(0),
      power_bat_current_2(0),
      power_sys_voltage_1(0),
      power_sys_voltage_2(0),
      power_sys_current_1(0),
      power_sys_current_2(0),
      power_comms_current(0),
      power_fs_current(0),
      power_cdh_current(0),
      power_util_current(0),
      power_bat_temp_1(0),
      power_bat_temp_2(0),
      power_panel_temp_1(0),
      power_panel_temp_2(0),
      power_eps_temp_1(0),
      power_eps_temp_2(0),
      power_charger_state_1(0),
      power_charger_state_2(0),
      power_charge_1(0),
      power_charge_2(0),
      power_system_1(0),
      power_system_2(0),
      power_soc_1(0),
      power_soc_2(0) {
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
        .AddData<float>(fs_angular_velocity_1)
        .AddData<float>(fs_angular_velocity_2)
        .AddData<float>(fs_angular_velocity_3)
        .AddData<uint8_t>(fs_attitude_cov)
        .AddData<uint8_t>(fs_attitude_distance)
        .AddData<byte>(fs_torquer_temp_1)
        .AddData<byte>(fs_torquer_temp_2)
        .AddData<byte>(fs_torquer_temp_3)
        .AddData<byte>(fs_flags)
        .AddEtlArray<byte, kFsFsAttitudeFlagByteCount>(fs_attitude_flags)
        .AddEtlArray<byte, kFsHealthFlagByteCount>(fs_health_flags)
        .AddEtlArray<float, kFsLocationFloatCount>(fs_location)
        .AddData<float>(fs_nadir_1)
        .AddData<float>(fs_nadir_2)
        .AddData<float>(fs_nadir_3)
        .AddData<uint8_t>(fs_ir)
        .AddData<uint8_t>(fs_control_1)
        .AddData<uint8_t>(fs_control_2)
        .AddData<uint8_t>(fs_control_3)
        .AddData<float>(fs_torquer_current_1)
        .AddData<float>(fs_torquer_current_2)
        .AddData<float>(fs_torquer_current_3)
        .AddData<uint16_t>(cdh_mcu_1)
        .AddData<uint16_t>(cdh_mcu_2)
        .AddData<uint64_t>(cdh_time.timestamp_millis_unix_epoch)
        .AddData<uint8_t>(mcu_reset_count_1)
        .AddData<uint8_t>(mcu_reset_count_2)
        .AddData<uint16_t>(cdh_memory)
        .AddData<uint16_t>(cdh_memory_available)
        .AddData<byte>(cdh_mcu_temp_1)
        .AddData<byte>(cdh_mcu_temp_2)
        .AddData<byte>(cdh_reg_voltage_1)
        .AddData<byte>(cdh_reg_current_1)
        .AddData<byte>(cdh_reg_temp_1)
        .AddData<byte>(cdh_reg_voltage_2)
        .AddData<byte>(cdh_reg_current_2)
        .AddData<byte>(cdh_reg_temp_2)
        .AddData<byte>(cdh_reg_voltage_3)
        .AddData<byte>(cdh_reg_current_3)
        .AddData<byte>(cdh_reg_temp_3)
        .AddData<uint16_t>(cdh_bus_fault)
        .AddData<uint64_t>(cdh_last_reboot.timestamp_millis_unix_epoch)
        .AddData<byte>(cdh_rtc_status)
        .AddData<uint16_t>(cdh_heartbeat);

    for (uint8_t i = 0; i < kOutreachMessageSize; i++) {
        builder.AddData<char>(comms_outreach[i]);
    }

    builder.AddData<byte>(comms_temp_1)
        .AddData<byte>(comms_temp_2)
        .AddData<byte>(comms_current_1)
        .AddData<byte>(comms_current_2)
        .AddData<byte>(comms_current_3)
        .AddData<byte>(comms_voltage_1)
        .AddData<byte>(comms_voltage_2)
        .AddData<uint16_t>(comms_lithium_op_count)
        .AddData<uint16_t>(comms_lithium_temp)
        .AddData<byte>(comms_lithium_time)
        .AddData<uint8_t>(comms_lithium_rssi)
        .AddData<uint32_t>(comms_bytes_received)
        .AddData<uint32_t>(comms_bytes_transmitted)
        .AddData<byte>(comms_antenna_flags)
        .AddData<byte>(power_solar_voltage_1)
        .AddData<byte>(power_solar_voltage_2)
        .AddData<byte>(power_solar_voltage_3)
        .AddData<byte>(power_solar_voltage_4)
        .AddData<byte>(power_solar_voltage_5)
        .AddData<byte>(power_solar_voltage_6)
        .AddData<byte>(power_solar_current_1)
        .AddData<byte>(power_solar_current_2)
        .AddData<byte>(power_solar_current_3)
        .AddData<byte>(power_solar_current_4)
        .AddData<byte>(power_solar_current_5)
        .AddData<byte>(power_solar_current_6)
        .AddData<byte>(power_panel_current_1)
        .AddData<byte>(power_panel_current_2)
        .AddData<byte>(power_panel_current_3)
        .AddData<byte>(power_panel_current_4)
        .AddData<byte>(power_panel_current_5)
        .AddData<byte>(power_panel_current_6)
        .AddData<uint16_t>(power_in_voltage_1)
        .AddData<uint16_t>(power_in_voltage_2)
        .AddData<uint16_t>(power_in_current_1)
        .AddData<uint16_t>(power_in_current_2)
        .AddData<uint16_t>(power_bat_voltage_1)
        .AddData<uint16_t>(power_bat_voltage_2)
        .AddData<uint16_t>(power_bat_current_1)
        .AddData<uint16_t>(power_bat_current_2)
        .AddData<uint16_t>(power_sys_voltage_1)
        .AddData<uint16_t>(power_sys_voltage_2)
        .AddData<byte>(power_sys_current_1)
        .AddData<byte>(power_sys_current_2)
        .AddData<byte>(power_comms_current)
        .AddData<byte>(power_fs_current)
        .AddData<byte>(power_cdh_current)
        .AddData<byte>(power_util_current)
        .AddData<uint16_t>(power_bat_temp_1)
        .AddData<uint16_t>(power_bat_temp_2)
        .AddData<byte>(power_panel_temp_1)
        .AddData<byte>(power_panel_temp_2)
        .AddData<byte>(power_eps_temp_1)
        .AddData<byte>(power_eps_temp_2)
        .AddData<uint16_t>(power_charger_state_1)
        .AddData<uint16_t>(power_charger_state_2)
        .AddData<uint16_t>(power_charge_1)
        .AddData<uint16_t>(power_charge_2)
        .AddData<uint16_t>(power_system_1)
        .AddData<uint16_t>(power_system_2)
        .AddData<uint16_t>(power_soc_1)
        .AddData<uint16_t>(power_soc_2);

    builder.PadZeroes();
    return builder.Build();
}

uint16_t BeaconPayload::GetSerialisedSize() const { return kPayloadSize; }

uint8_t BeaconPayload::GetPayloadCode() const { return kBeaconPayloadCode; }

BeaconPayload* BeaconPayload::SetCdhBusFault(uint16_t cdhBusFault) {
    this->cdh_bus_fault = cdhBusFault;
    return this;
}

BeaconPayload* BeaconPayload::SetCdhHeartbeat(uint16_t cdhHeartbeat) {
    this->cdh_heartbeat = cdhHeartbeat;
    return this;
}

BeaconPayload* BeaconPayload::SetCdhLastReboot(const Time& cdhLastReboot) {
    this->cdh_last_reboot = cdhLastReboot;
    return this;
}

BeaconPayload* BeaconPayload::SetCdhMcuTemp1(byte cdhMcuTemp1) {
    this->cdh_mcu_temp_1 = cdhMcuTemp1;
    return this;
}

BeaconPayload* BeaconPayload::SetCdhMcuTemp2(byte cdhMcuTemp2) {
    this->cdh_mcu_temp_2 = cdhMcuTemp2;
    return this;
}

BeaconPayload* BeaconPayload::SetCdhMemory(uint16_t cdhMemory) {
    this->cdh_memory = cdhMemory;
    return this;
}

BeaconPayload* BeaconPayload::SetCdhMemoryAvailable(
    uint16_t cdhMemoryAvailable) {
    this->cdh_memory_available = cdhMemoryAvailable;
    return this;
}

BeaconPayload* BeaconPayload::SetCdhRegCurrent1(byte cdhRegCurrent1) {
    this->cdh_reg_current_1 = cdhRegCurrent1;
    return this;
}

BeaconPayload* BeaconPayload::SetCdhRegCurrent2(byte cdhRegCurrent2) {
    this->cdh_reg_current_2 = cdhRegCurrent2;
    return this;
}

BeaconPayload* BeaconPayload::SetCdhRegCurrent3(byte cdhRegCurrent3) {
    this->cdh_reg_current_3 = cdhRegCurrent3;
    return this;
}

BeaconPayload* BeaconPayload::SetCdhRegTemp1(byte cdhRegTemp1) {
    this->cdh_reg_temp_1 = cdhRegTemp1;
    return this;
}

BeaconPayload* BeaconPayload::SetCdhRegTemp2(byte cdhRegTemp2) {
    this->cdh_reg_temp_2 = cdhRegTemp2;
    return this;
}

BeaconPayload* BeaconPayload::SetCdhRegTemp3(byte cdhRegTemp3) {
    this->cdh_reg_temp_3 = cdhRegTemp3;
    return this;
}

BeaconPayload* BeaconPayload::SetCdhRegVoltage1(byte cdhRegVoltage1) {
    this->cdh_reg_voltage_1 = cdhRegVoltage1;
    return this;
}

BeaconPayload* BeaconPayload::SetCdhRegVoltage2(byte cdhRegVoltage2) {
    this->cdh_reg_voltage_2 = cdhRegVoltage2;
    return this;
}

BeaconPayload* BeaconPayload::SetCdhRegVoltage3(byte cdhRegVoltage3) {
    this->cdh_reg_voltage_3 = cdhRegVoltage3;
    return this;
}

BeaconPayload* BeaconPayload::SetCdhRtcStatus(byte cdhRtcStatus) {
    this->cdh_rtc_status = cdhRtcStatus;
    return this;
}

BeaconPayload* BeaconPayload::SetCdhMcu1(uint16_t cdhMcu1) {
    this->cdh_mcu_1 = cdhMcu1;
    return this;
}

BeaconPayload* BeaconPayload::SetCdhMcu2(uint16_t cdhMcu2) {
    this->cdh_mcu_2 = cdhMcu2;
    return this;
}

BeaconPayload* BeaconPayload::SetCdhTime(const Time& cdhTime) {
    this->cdh_time = cdhTime;
    return this;
}

BeaconPayload* BeaconPayload::SetCommsAntennaFlags(byte commsAntennaFlags) {
    this->comms_antenna_flags = commsAntennaFlags;
    return this;
}

BeaconPayload* BeaconPayload::SetCommsBytesReceived(
    uint32_t commsBytesReceived) {
    this->comms_bytes_received = commsBytesReceived;
    return this;
}

BeaconPayload* BeaconPayload::SetCommsBytesTransmitted(
    uint32_t commsBytesTransmitted) {
    this->comms_bytes_transmitted = commsBytesTransmitted;
    return this;
}

BeaconPayload* BeaconPayload::SetCommsCurrent1(byte commsCurrent1) {
    this->comms_current_1 = commsCurrent1;
    return this;
}

BeaconPayload* BeaconPayload::SetCommsCurrent2(byte commsCurrent2) {
    this->comms_current_2 = commsCurrent2;
    return this;
}

BeaconPayload* BeaconPayload::SetCommsCurrent3(byte commsCurrent3) {
    this->comms_current_3 = commsCurrent3;
    return this;
}

BeaconPayload* BeaconPayload::SetCommsLithiumOpCount(
    uint16_t commsLithiumOpCount) {
    this->comms_lithium_op_count = commsLithiumOpCount;
    return this;
}

BeaconPayload* BeaconPayload::SetCommsLithiumRssi(uint16_t commsLithiumRssi) {
    this->comms_lithium_rssi = commsLithiumRssi;
    return this;
}

BeaconPayload* BeaconPayload::SetCommsLithiumTemp(uint16_t commsLithiumTemp) {
    this->comms_lithium_temp = commsLithiumTemp;
    return this;
}

BeaconPayload* BeaconPayload::SetCommsLithiumTime(byte commsLithiumTime) {
    this->comms_lithium_time = commsLithiumTime;
    return this;
}

BeaconPayload* BeaconPayload::SetCommsTemp1(byte commsTemp1) {
    this->comms_temp_1 = commsTemp1;
    return this;
}

BeaconPayload* BeaconPayload::SetCommsTemp2(byte commsTemp2) {
    this->comms_temp_2 = commsTemp2;
    return this;
}

BeaconPayload* BeaconPayload::SetCommsVoltage1(byte commsVoltage1) {
    this->comms_voltage_1 = commsVoltage1;
    return this;
}

BeaconPayload* BeaconPayload::SetCommsVoltage2(byte commsVoltage2) {
    this->comms_voltage_2 = commsVoltage2;
    return this;
}

BeaconPayload* BeaconPayload::SetFsAngularVelocity1(float fsAngularVelocity1) {
    this->fs_angular_velocity_1 = fsAngularVelocity1;
    return this;
}

BeaconPayload* BeaconPayload::SetFsAngularVelocity2(float fsAngularVelocity2) {
    this->fs_angular_velocity_2 = fsAngularVelocity2;
    return this;
}

BeaconPayload* BeaconPayload::SetFsAngularVelocity3(float fsAngularVelocity3) {
    this->fs_angular_velocity_3 = fsAngularVelocity3;
    return this;
}

BeaconPayload* BeaconPayload::SetFsAttitudeCov(uint8_t fsAttitudeCov) {
    this->fs_attitude_cov = fsAttitudeCov;
    return this;
}

BeaconPayload* BeaconPayload::SetFsAttitudeDistance(
    uint8_t fsAttitudeDistance) {
    this->fs_attitude_distance = fsAttitudeDistance;
    return this;
}

BeaconPayload* BeaconPayload::SetFsAttitudeFlags(
    etl::array<byte, kFsFsAttitudeFlagByteCount> fsAttitudeFlags) {
    this->fs_attitude_flags = fsAttitudeFlags;
    return this;
}

BeaconPayload* BeaconPayload::SetFsAttitudeQuaternion1(
    float fsAttitudeQuaternion1) {
    this->fs_attitude_quaternion_1 = fsAttitudeQuaternion1;
    return this;
}

BeaconPayload* BeaconPayload::SetFsAttitudeQuaternion2(
    float fsAttitudeQuaternion2) {
    this->fs_attitude_quaternion_2 = fsAttitudeQuaternion2;
    return this;
}

BeaconPayload* BeaconPayload::SetFsAttitudeQuaternion3(
    float fsAttitudeQuaternion3) {
    this->fs_attitude_quaternion_3 = fsAttitudeQuaternion3;
    return this;
}

BeaconPayload* BeaconPayload::SetFsAttitudeQuaternion4(
    float fsAttitudeQuaternion4) {
    this->fs_attitude_quaternion_4 = fsAttitudeQuaternion4;
    return this;
}

BeaconPayload* BeaconPayload::SetFsControl1(uint8_t fsControl1) {
    this->fs_control_1 = fsControl1;
    return this;
}

BeaconPayload* BeaconPayload::SetFsControl2(uint8_t fsControl2) {
    this->fs_control_2 = fsControl2;
    return this;
}

BeaconPayload* BeaconPayload::SetFsControl3(uint8_t fsControl3) {
    this->fs_control_3 = fsControl3;
    return this;
}

BeaconPayload* BeaconPayload::SetFsFlags(byte fsFlags) {
    this->fs_flags = fsFlags;
    return this;
}

BeaconPayload* BeaconPayload::SetFsHealthFlags(
    etl::array<byte, kFsHealthFlagByteCount> fsHealthFlags) {
    this->fs_health_flags = fsHealthFlags;
    return this;
}

BeaconPayload* BeaconPayload::SetFsNadir1(float fsNadir1) {
    this->fs_nadir_1 = fsNadir1;
    return this;
}

BeaconPayload* BeaconPayload::SetFsNadir2(float fsNadir2) {
    this->fs_nadir_2 = fsNadir2;
    return this;
}

BeaconPayload* BeaconPayload::SetFsNadir3(float fsNadir3) {
    this->fs_nadir_3 = fsNadir3;
    return this;
}

BeaconPayload* BeaconPayload::SetFsTorquerCurrent1(float fsTorquerCurrent1) {
    this->fs_torquer_current_1 = fsTorquerCurrent1;
    return this;
}

BeaconPayload* BeaconPayload::SetFsTorquerCurrent2(float fsTorquerCurrent2) {
    this->fs_torquer_current_2 = fsTorquerCurrent2;
    return this;
}

BeaconPayload* BeaconPayload::SetFsTorquerCurrent3(float fsTorquerCurrent3) {
    this->fs_torquer_current_3 = fsTorquerCurrent3;
    return this;
}

BeaconPayload* BeaconPayload::SetFsTorquerTemp1(byte fsTorquerTemp1) {
    this->fs_torquer_temp_1 = fsTorquerTemp1;
    return this;
}

BeaconPayload* BeaconPayload::SetFsTorquerTemp2(byte fsTorquerTemp2) {
    this->fs_torquer_temp_2 = fsTorquerTemp2;
    return this;
}

BeaconPayload* BeaconPayload::SetFsTorquerTemp3(byte fsTorquerTemp3) {
    this->fs_torquer_temp_3 = fsTorquerTemp3;
    return this;
}

BeaconPayload* BeaconPayload::SetMcuResetCount1(uint8_t mcuResetCount1) {
    mcu_reset_count_1 = mcuResetCount1;
    return this;
}

BeaconPayload* BeaconPayload::SetMcuResetCount2(uint8_t mcuResetCount2) {
    mcu_reset_count_2 = mcuResetCount2;
    return this;
}

BeaconPayload* BeaconPayload::SetPowerBatCurrent1(uint16_t powerBatCurrent1) {
    this->power_bat_current_1 = powerBatCurrent1;
    return this;
}

BeaconPayload* BeaconPayload::SetPowerBatCurrent2(uint16_t powerBatCurrent2) {
    this->power_bat_current_2 = powerBatCurrent2;
    return this;
}

BeaconPayload* BeaconPayload::SetPowerBatTemp1(uint16_t powerBatTemp1) {
    this->power_bat_temp_1 = powerBatTemp1;
    return this;
}

BeaconPayload* BeaconPayload::SetPowerBatTemp2(uint16_t powerBatTemp2) {
    this->power_bat_temp_2 = powerBatTemp2;
    return this;
}

BeaconPayload* BeaconPayload::SetPowerBatVoltage1(uint16_t powerBatVoltage1) {
    this->power_bat_voltage_1 = powerBatVoltage1;
    return this;
}

BeaconPayload* BeaconPayload::SetPowerBatVoltage2(uint16_t powerBatVoltage2) {
    this->power_bat_voltage_2 = powerBatVoltage2;
    return this;
}

BeaconPayload* BeaconPayload::SetPowerCdhCurrent(byte powerCdhCurrent) {
    this->power_cdh_current = powerCdhCurrent;
    return this;
}

BeaconPayload* BeaconPayload::SetPowerCharge1(uint16_t powerCharge1) {
    this->power_charge_1 = powerCharge1;
    return this;
}

BeaconPayload* BeaconPayload::SetPowerCharge2(uint16_t powerCharge2) {
    this->power_charge_2 = powerCharge2;
    return this;
}

BeaconPayload* BeaconPayload::SetPowerChargerState1(
    uint16_t powerChargerState1) {
    this->power_charger_state_1 = powerChargerState1;
    return this;
}

BeaconPayload* BeaconPayload::SetPowerChargerState2(
    uint16_t powerChargerState2) {
    this->power_charger_state_2 = powerChargerState2;
    return this;
}

BeaconPayload* BeaconPayload::SetPowerCommsCurrent(byte powerCommsCurrent) {
    this->power_comms_current = powerCommsCurrent;
    return this;
}

BeaconPayload* BeaconPayload::SetPowerEpsTemp1(byte powerEpsTemp1) {
    this->power_eps_temp_1 = powerEpsTemp1;
    return this;
}

BeaconPayload* BeaconPayload::SetPowerEpsTemp2(byte powerEpsTemp2) {
    this->power_eps_temp_2 = powerEpsTemp2;
    return this;
}

BeaconPayload* BeaconPayload::SetPowerFsCurrent(byte powerFsCurrent) {
    this->power_fs_current = powerFsCurrent;
    return this;
}

BeaconPayload* BeaconPayload::SetPowerInCurrent1(uint16_t powerInCurrent1) {
    this->power_in_current_1 = powerInCurrent1;
    return this;
}

BeaconPayload* BeaconPayload::SetPowerInCurrent2(uint16_t powerInCurrent2) {
    this->power_in_current_2 = powerInCurrent2;
    return this;
}

BeaconPayload* BeaconPayload::SetPowerInVoltage1(uint16_t powerInVoltage1) {
    this->power_in_voltage_1 = powerInVoltage1;
    return this;
}

BeaconPayload* BeaconPayload::SetPowerInVoltage2(uint16_t powerInVoltage2) {
    this->power_in_voltage_2 = powerInVoltage2;
    return this;
}

BeaconPayload* BeaconPayload::SetPowerPanelCurrent1(byte powerPanelCurrent1) {
    this->power_panel_current_1 = powerPanelCurrent1;
    return this;
}

BeaconPayload* BeaconPayload::SetPowerPanelCurrent2(byte powerPanelCurrent2) {
    this->power_panel_current_2 = powerPanelCurrent2;
    return this;
}

BeaconPayload* BeaconPayload::SetPowerPanelCurrent3(byte powerPanelCurrent3) {
    this->power_panel_current_3 = powerPanelCurrent3;
    return this;
}

BeaconPayload* BeaconPayload::SetPowerPanelCurrent4(byte powerPanelCurrent4) {
    this->power_panel_current_4 = powerPanelCurrent4;
    return this;
}

BeaconPayload* BeaconPayload::SetPowerPanelCurrent5(byte powerPanelCurrent5) {
    this->power_panel_current_5 = powerPanelCurrent5;
    return this;
}

BeaconPayload* BeaconPayload::SetPowerPanelCurrent6(byte powerPanelCurrent6) {
    this->power_panel_current_6 = powerPanelCurrent6;
    return this;
}

BeaconPayload* BeaconPayload::SetPowerPanelTemp1(byte powerPanelTemp1) {
    this->power_panel_temp_1 = powerPanelTemp1;
    return this;
}

BeaconPayload* BeaconPayload::SetPowerPanelTemp2(byte powerPanelTemp2) {
    this->power_panel_temp_2 = powerPanelTemp2;
    return this;
}

BeaconPayload* BeaconPayload::SetPowerSoc1(uint16_t powerSoc1) {
    this->power_soc_1 = powerSoc1;
    return this;
}

BeaconPayload* BeaconPayload::SetPowerSoc2(uint16_t powerSoc2) {
    this->power_soc_2 = powerSoc2;
    return this;
}

BeaconPayload* BeaconPayload::SetPowerSolarCurrent1(byte powerSolarCurrent1) {
    this->power_solar_current_1 = powerSolarCurrent1;
    return this;
}

BeaconPayload* BeaconPayload::SetPowerSolarCurrent2(byte powerSolarCurrent2) {
    this->power_solar_current_2 = powerSolarCurrent2;
    return this;
}

BeaconPayload* BeaconPayload::SetPowerSolarCurrent3(byte powerSolarCurrent3) {
    this->power_solar_current_3 = powerSolarCurrent3;
    return this;
}

BeaconPayload* BeaconPayload::SetPowerSolarCurrent4(byte powerSolarCurrent4) {
    this->power_solar_current_4 = powerSolarCurrent4;
    return this;
}

BeaconPayload* BeaconPayload::SetPowerSolarCurrent5(byte powerSolarCurrent5) {
    this->power_solar_current_5 = powerSolarCurrent5;
    return this;
}

BeaconPayload* BeaconPayload::SetPowerSolarCurrent6(byte powerSolarCurrent6) {
    this->power_solar_current_6 = powerSolarCurrent6;
    return this;
}

BeaconPayload* BeaconPayload::SetPowerSolarVoltage1(byte powerSolarVoltage1) {
    this->power_solar_voltage_1 = powerSolarVoltage1;
    return this;
}

BeaconPayload* BeaconPayload::SetPowerSolarVoltage2(byte powerSolarVoltage2) {
    this->power_solar_voltage_2 = powerSolarVoltage2;
    return this;
}

BeaconPayload* BeaconPayload::SetPowerSolarVoltage3(byte powerSolarVoltage3) {
    this->power_solar_voltage_3 = powerSolarVoltage3;
    return this;
}

BeaconPayload* BeaconPayload::SetPowerSolarVoltage4(byte powerSolarVoltage4) {
    this->power_solar_voltage_4 = powerSolarVoltage4;
    return this;
}

BeaconPayload* BeaconPayload::SetPowerSolarVoltage5(byte powerSolarVoltage5) {
    this->power_solar_voltage_5 = powerSolarVoltage5;
    return this;
}

BeaconPayload* BeaconPayload::SetPowerSolarVoltage6(byte powerSolarVoltage6) {
    this->power_solar_voltage_6 = powerSolarVoltage6;
    return this;
}

BeaconPayload* BeaconPayload::SetPowerSysCurrent1(byte powerSysCurrent1) {
    this->power_sys_current_1 = powerSysCurrent1;
    return this;
}

BeaconPayload* BeaconPayload::SetPowerSysCurrent2(byte powerSysCurrent2) {
    this->power_sys_current_2 = powerSysCurrent2;
    return this;
}

BeaconPayload* BeaconPayload::SetPowerSysVoltage1(uint16_t powerSysVoltage1) {
    this->power_sys_voltage_1 = powerSysVoltage1;
    return this;
}

BeaconPayload* BeaconPayload::SetPowerSysVoltage2(uint16_t powerSysVoltage2) {
    this->power_sys_voltage_2 = powerSysVoltage2;
    return this;
}

BeaconPayload* BeaconPayload::SetPowerSystem1(uint16_t powerSystem1) {
    this->power_system_1 = powerSystem1;
    return this;
}

BeaconPayload* BeaconPayload::SetPowerSystem2(uint16_t powerSystem2) {
    this->power_system_2 = powerSystem2;
    return this;
}

BeaconPayload* BeaconPayload::SetPowerUtilCurrent(byte powerUtilCurrent) {
    this->power_util_current = powerUtilCurrent;
    return this;
}

BeaconPayload* BeaconPayload::SetFsLocation(
    etl::array<float, kFsLocationFloatCount> fsLocation) {
    this->fs_location = fsLocation;
    return this;
}

BeaconPayload* BeaconPayload::SetFsIr(uint8_t fsIr) {
    this->fs_ir = fsIr;
    return this;
}
