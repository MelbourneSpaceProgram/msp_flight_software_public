#include <src/database/flash_memory/flash_storables/system_configuration.h>
#include <src/messages/SystemConfigurationUplinkPayload.pb.h>
#include <xdc/runtime/Log.h>

SystemConfiguration *SystemConfiguration::GetInstance() {
    static SystemConfiguration *instance = nullptr;
    if (instance == nullptr) {
        instance = new SystemConfiguration(true, false);
    }
    return instance;
}

SystemConfiguration::SystemConfiguration() : SystemConfiguration(true, false) {}

SystemConfiguration::SystemConfiguration(bool, bool use_defaults)
    : FlashStorable<SystemConfiguration>(
          kSystemConfigurationFlashStorageAddress) {
    if (use_defaults) return;
    SystemConfiguration test_configuration;
    test_configuration.UpdateFromFlash();
    if (test_configuration.checksum == test_configuration.ComputeChecksum()) {
        // If the test configuration is different to this, could potentially
        // fail
        UpdateFromFlash();
    } else {
        Log_error0(
            "Invalid system configuration found in flash memory, resetting to "
            "defaults");
        StoreInFlash();
    }
}

SystemConfiguration::SystemConfiguration(
    SystemConfigurationUplinkPayload &payload)
    : FlashStorable<SystemConfiguration>(
          kSystemConfigurationFlashStorageAddress),
      eps_board_available(payload.eps_board_available),
      cdh_board_available(payload.cdh_board_available),
      fs_board_available(payload.fs_board_available),
      tcom_board_available(payload.tcom_board_available),
      utilities_board_available(payload.utilities_board_available),
      x_pos_available(payload.x_pos_available),
      x_neg_available(payload.x_neg_available),
      y_pos_available(payload.y_pos_available),
      y_neg_available(payload.y_neg_available),
      z_pos_available(payload.z_pos_available),
      z_neg_available(payload.z_neg_available),
      i2c_available(payload.i2c_available),
      sd_card_available(payload.sd_card_available),
      bms_available(payload.bms_available),
      antenna_available(payload.antenna_available),
      eeprom_available(payload.eeprom_available),
      enter_deep_sleep_on_startup(payload.enter_deep_sleep_on_startup),
      enter_limp_mode_on_startup(payload.enter_limp_mode_on_startup),
      deploy_antenna(payload.deploy_antenna),
      instant_deployment_waits(payload.instant_deployment_waits),
      format_sd_on_startup(payload.format_sd_on_startup),
      beacon_wait_ms(payload.beacon_wait_ms),
      ejection_wait_ms(payload.ejection_wait_ms),
      time_source_deploy_ms(payload.time_source_deploy_ms),
      lithium_transmit_only_when_ground_commanded(
          payload.lithium_transmit_only_when_ground_commanded),
      nominal_lithium_power_level(payload.nominal_lithium_power_level),
      uplink_fec_enabled(payload.uplink_fec_enabled),
      downlink_fec_enabled(payload.downlink_fec_enabled),
      check_hmac_default(payload.check_hmac_default),
      check_sequence_default(payload.check_sequence_default),
      log_i2c(payload.log_i2c),
      log_to_uart(payload.log_to_uart),
      log_to_sd(payload.log_to_sd),
      hil_available(payload.hil_available),
      ditl_mode(payload.ditl_mode),
      verbose_logging(payload.verbose_logging),
      reboot_on_top_level_exception(payload.reboot_on_top_level_exception),
      nominal_beacon_period_ms(payload.nominal_beacon_period_ms),
      orientation_control_loop_period_micros(
          payload.orientation_control_loop_period_micros),
      health_check_period_ms(payload.health_check_period_ms),
      antenna_burn_check_interval_ms(payload.antenna_burn_check_interval_ms),
      time_update_period_ms(payload.time_update_period_ms),
      i_charge_hold_time_ms(payload.i_charge_hold_time_ms),
      i_charge_hold_poll_ms(payload.i_charge_hold_poll_ms),
      solar_power_recovery_time_ms(payload.solar_power_recovery_time_ms),
      initial_i_charge_index(payload.initial_i_charge_index),
      half_orbit_period_ms(payload.half_orbit_period_ms),
      initial_antenna_burn_interval_ms(
          payload.initial_antenna_burn_interval_ms),
      backup_antenna_burn_interval_ms(payload.backup_antenna_burn_interval_ms),
      antenna_burn_interval_multiplier(
          payload.antenna_burn_interval_multiplier),
      run_magnetorquers_at_constant_power(
          payload.run_magnetorquers_at_constant_power),
      magnetorquer_power_fraction_x(payload.magnetorquer_power_fraction_x),
      magnetorquer_power_fraction_y(payload.magnetorquer_power_fraction_y),
      magnetorquer_power_fraction_z(payload.magnetorquer_power_fraction_z),
      max_magnetorquer_dipole({payload.max_magnetorquer_dipole_1,
                               payload.max_magnetorquer_dipole_2,
                               payload.max_magnetorquer_dipole_3}),
      orientation_control_power_level(payload.orientation_control_power_level),
      checksum(payload.checksum) {}

uint32_t SystemConfiguration::ComputeChecksum() {
    // TODO(dingbenjamin): Make a better checksum
    return eps_board_available + cdh_board_available + fs_board_available +
           tcom_board_available + utilities_board_available + x_pos_available +
           x_neg_available + y_pos_available + y_neg_available +
           z_pos_available + z_neg_available + i2c_available +
           sd_card_available + bms_available + antenna_available +
           eeprom_available + enter_deep_sleep_on_startup +
           enter_limp_mode_on_startup + deploy_antenna +
           instant_deployment_waits + format_sd_on_startup +
           lithium_transmit_only_when_ground_commanded + uplink_fec_enabled +
           downlink_fec_enabled + check_hmac_default + check_sequence_default +
           log_i2c + log_to_uart + log_to_sd + hil_available + ditl_mode +
           verbose_logging + reboot_on_top_level_exception +
           run_magnetorquers_at_constant_power + nominal_lithium_power_level +
           i_charge_hold_time_ms + i_charge_hold_poll_ms +
           initial_i_charge_index + beacon_wait_ms + ejection_wait_ms +
           nominal_beacon_period_ms + orientation_control_loop_period_micros +
           health_check_period_ms + time_update_period_ms +
           solar_power_recovery_time_ms + time_source_deploy_ms +
           antenna_burn_check_interval_ms + half_orbit_period_ms +
           initial_antenna_burn_interval_ms + backup_antenna_burn_interval_ms +
           antenna_burn_interval_multiplier * 10 +
           magnetorquer_power_fraction_x * 10 +
           magnetorquer_power_fraction_y * 10 +
           magnetorquer_power_fraction_z * 10 +
           max_magnetorquer_dipole[0] * 10 + max_magnetorquer_dipole[1] * 10 +
           max_magnetorquer_dipole[2] * 10 +
           orientation_control_power_level * 10;
}

void SystemConfiguration::UpdateFromFlashStorableStruct(
    SystemConfiguration *system_configuration_struct) {
    eps_board_available = system_configuration_struct->eps_board_available;
    cdh_board_available = system_configuration_struct->cdh_board_available;
    fs_board_available = system_configuration_struct->fs_board_available;
    tcom_board_available = system_configuration_struct->tcom_board_available;
    utilities_board_available =
        system_configuration_struct->utilities_board_available;
    x_pos_available = system_configuration_struct->x_pos_available;
    x_neg_available = system_configuration_struct->x_neg_available;
    y_pos_available = system_configuration_struct->y_pos_available;
    y_neg_available = system_configuration_struct->y_neg_available;
    z_pos_available = system_configuration_struct->z_pos_available;
    z_neg_available = system_configuration_struct->z_neg_available;
    i2c_available = system_configuration_struct->i2c_available;
    sd_card_available = system_configuration_struct->sd_card_available;
    bms_available = system_configuration_struct->bms_available;
    antenna_available = system_configuration_struct->antenna_available;
    eeprom_available = system_configuration_struct->eeprom_available;
    enter_deep_sleep_on_startup =
        system_configuration_struct->enter_deep_sleep_on_startup;
    enter_limp_mode_on_startup =
        system_configuration_struct->enter_limp_mode_on_startup;
    deploy_antenna = system_configuration_struct->deploy_antenna;
    instant_deployment_waits =
        system_configuration_struct->instant_deployment_waits;
    format_sd_on_startup = system_configuration_struct->format_sd_on_startup;
    beacon_wait_ms = system_configuration_struct->beacon_wait_ms;
    ejection_wait_ms = system_configuration_struct->ejection_wait_ms;
    time_source_deploy_ms = system_configuration_struct->time_source_deploy_ms;
    lithium_transmit_only_when_ground_commanded =
        system_configuration_struct
            ->lithium_transmit_only_when_ground_commanded;
    nominal_lithium_power_level =
        system_configuration_struct->nominal_lithium_power_level;
    uplink_fec_enabled = system_configuration_struct->uplink_fec_enabled;
    downlink_fec_enabled = system_configuration_struct->downlink_fec_enabled;
    check_hmac_default = system_configuration_struct->check_hmac_default;
    check_sequence_default =
        system_configuration_struct->check_sequence_default;
    log_i2c = system_configuration_struct->log_i2c;
    log_to_uart = system_configuration_struct->log_to_uart;
    log_to_sd = system_configuration_struct->log_to_sd;
    hil_available = system_configuration_struct->hil_available;
    ditl_mode = system_configuration_struct->ditl_mode;
    verbose_logging = system_configuration_struct->verbose_logging;
    reboot_on_top_level_exception =
        system_configuration_struct->reboot_on_top_level_exception;
    nominal_beacon_period_ms =
        system_configuration_struct->nominal_beacon_period_ms;
    orientation_control_loop_period_micros =
        system_configuration_struct->orientation_control_loop_period_micros;
    health_check_period_ms =
        system_configuration_struct->health_check_period_ms;
    antenna_burn_check_interval_ms =
        system_configuration_struct->antenna_burn_check_interval_ms;
    time_update_period_ms = system_configuration_struct->time_update_period_ms;
    i_charge_hold_time_ms = system_configuration_struct->i_charge_hold_time_ms;
    i_charge_hold_poll_ms = system_configuration_struct->i_charge_hold_poll_ms;
    solar_power_recovery_time_ms =
        system_configuration_struct->solar_power_recovery_time_ms;
    initial_i_charge_index =
        system_configuration_struct->initial_i_charge_index;
    half_orbit_period_ms = system_configuration_struct->half_orbit_period_ms;
    initial_antenna_burn_interval_ms =
        system_configuration_struct->initial_antenna_burn_interval_ms;
    backup_antenna_burn_interval_ms =
        system_configuration_struct->backup_antenna_burn_interval_ms;
    antenna_burn_interval_multiplier =
        system_configuration_struct->antenna_burn_interval_multiplier;
    run_magnetorquers_at_constant_power =
        system_configuration_struct->run_magnetorquers_at_constant_power;
    magnetorquer_power_fraction_x =
        system_configuration_struct->magnetorquer_power_fraction_x;
    magnetorquer_power_fraction_y =
        system_configuration_struct->magnetorquer_power_fraction_y;
    magnetorquer_power_fraction_z =
        system_configuration_struct->magnetorquer_power_fraction_z;
    max_magnetorquer_dipole[0] =
        system_configuration_struct->max_magnetorquer_dipole[0];
    max_magnetorquer_dipole[1] =
        system_configuration_struct->max_magnetorquer_dipole[1];
    max_magnetorquer_dipole[2] =
        system_configuration_struct->max_magnetorquer_dipole[2];
    orientation_control_power_level =
        system_configuration_struct->orientation_control_power_level;
}

void SystemConfiguration::ConvertToFlashStorableStruct(
    SystemConfiguration *system_configuration_struct) {
    system_configuration_struct->eps_board_available = eps_board_available;
    system_configuration_struct->cdh_board_available = cdh_board_available;
    system_configuration_struct->fs_board_available = fs_board_available;
    system_configuration_struct->tcom_board_available = tcom_board_available;
    system_configuration_struct->utilities_board_available =
        utilities_board_available;
    system_configuration_struct->x_pos_available = x_pos_available;
    system_configuration_struct->x_neg_available = x_neg_available;
    system_configuration_struct->y_pos_available = y_pos_available;
    system_configuration_struct->y_neg_available = y_neg_available;
    system_configuration_struct->z_pos_available = z_pos_available;
    system_configuration_struct->z_neg_available = z_neg_available;
    system_configuration_struct->i2c_available = i2c_available;
    system_configuration_struct->sd_card_available = sd_card_available;
    system_configuration_struct->bms_available = bms_available;
    system_configuration_struct->antenna_available = antenna_available;
    system_configuration_struct->eeprom_available = eeprom_available;
    system_configuration_struct->enter_deep_sleep_on_startup =
        enter_deep_sleep_on_startup;
    system_configuration_struct->enter_limp_mode_on_startup =
        enter_limp_mode_on_startup;
    system_configuration_struct->deploy_antenna = deploy_antenna;
    system_configuration_struct->instant_deployment_waits =
        instant_deployment_waits;
    system_configuration_struct->format_sd_on_startup = format_sd_on_startup;
    system_configuration_struct->beacon_wait_ms = beacon_wait_ms;
    system_configuration_struct->ejection_wait_ms = ejection_wait_ms;
    system_configuration_struct->time_source_deploy_ms = time_source_deploy_ms;
    system_configuration_struct->lithium_transmit_only_when_ground_commanded =
        lithium_transmit_only_when_ground_commanded;
    system_configuration_struct->nominal_lithium_power_level =
        nominal_lithium_power_level;
    system_configuration_struct->uplink_fec_enabled = uplink_fec_enabled;
    system_configuration_struct->downlink_fec_enabled = downlink_fec_enabled;
    system_configuration_struct->check_hmac_default = check_hmac_default;
    system_configuration_struct->check_sequence_default =
        check_sequence_default;
    system_configuration_struct->log_i2c = log_i2c;
    system_configuration_struct->log_to_uart = log_to_uart;
    system_configuration_struct->log_to_sd = log_to_sd;
    system_configuration_struct->hil_available = hil_available;
    system_configuration_struct->ditl_mode = ditl_mode;
    system_configuration_struct->verbose_logging = verbose_logging;
    system_configuration_struct->reboot_on_top_level_exception =
        reboot_on_top_level_exception;
    system_configuration_struct->nominal_beacon_period_ms =
        nominal_beacon_period_ms;
    system_configuration_struct->orientation_control_loop_period_micros =
        orientation_control_loop_period_micros;
    system_configuration_struct->health_check_period_ms =
        health_check_period_ms;
    system_configuration_struct->antenna_burn_check_interval_ms =
        antenna_burn_check_interval_ms;
    system_configuration_struct->time_update_period_ms = time_update_period_ms;
    system_configuration_struct->i_charge_hold_time_ms = i_charge_hold_time_ms;
    system_configuration_struct->i_charge_hold_poll_ms = i_charge_hold_poll_ms;
    system_configuration_struct->solar_power_recovery_time_ms =
        solar_power_recovery_time_ms;
    system_configuration_struct->initial_i_charge_index =
        initial_i_charge_index;
    system_configuration_struct->half_orbit_period_ms = half_orbit_period_ms;
    system_configuration_struct->initial_antenna_burn_interval_ms =
        initial_antenna_burn_interval_ms;
    system_configuration_struct->backup_antenna_burn_interval_ms =
        backup_antenna_burn_interval_ms;
    system_configuration_struct->antenna_burn_interval_multiplier =
        antenna_burn_interval_multiplier;
    system_configuration_struct->run_magnetorquers_at_constant_power =
        run_magnetorquers_at_constant_power;
    system_configuration_struct->magnetorquer_power_fraction_x =
        magnetorquer_power_fraction_x;
    system_configuration_struct->magnetorquer_power_fraction_y =
        magnetorquer_power_fraction_y;
    system_configuration_struct->magnetorquer_power_fraction_z =
        magnetorquer_power_fraction_z;
    system_configuration_struct->max_magnetorquer_dipole[0] =
        max_magnetorquer_dipole[0];
    system_configuration_struct->max_magnetorquer_dipole[1] =
        max_magnetorquer_dipole[1];
    system_configuration_struct->max_magnetorquer_dipole[2] =
        max_magnetorquer_dipole[2];
    system_configuration_struct->orientation_control_power_level =
        orientation_control_power_level;
}

bool SystemConfiguration::IsAntennaAvailable() const {
    return antenna_available;
}

uint64_t SystemConfiguration::GetAntennaBurnCheckIntervalMs() const {
    return antenna_burn_check_interval_ms;
}

float SystemConfiguration::GetAntennaBurnIntervalMultiplier() const {
    return antenna_burn_interval_multiplier;
}

uint64_t SystemConfiguration::GetBackupAntennaBurnIntervalMs() const {
    return backup_antenna_burn_interval_ms;
}

uint32_t SystemConfiguration::GetBeaconWaitMs() const { return beacon_wait_ms; }

bool SystemConfiguration::IsBmsAvailable() const { return bms_available; }

bool SystemConfiguration::IsCdhBoardAvailable() const {
    return cdh_board_available;
}

bool SystemConfiguration::IsCheckHmacDefault() const {
    return check_hmac_default;
}

bool SystemConfiguration::IsCheckSequenceDefault() const {
    return check_sequence_default;
}

bool SystemConfiguration::IsDeployAntenna() const { return deploy_antenna; }

bool SystemConfiguration::IsDitlMode() const { return ditl_mode; }

bool SystemConfiguration::IsDownlinkFecEnabled() const {
    return downlink_fec_enabled;
}

bool SystemConfiguration::IsEepromAvailable() const { return eeprom_available; }

uint32_t SystemConfiguration::GetEjectionWaitMs() const {
    return ejection_wait_ms;
}

bool SystemConfiguration::IsEnterDeepSleepOnStartup() const {
    return enter_deep_sleep_on_startup;
}

bool SystemConfiguration::IsEnterLimpModeOnStartup() const {
    return enter_limp_mode_on_startup;
}

bool SystemConfiguration::IsEpsBoardAvailable() const {
    return eps_board_available;
}

bool SystemConfiguration::IsFormatSdOnStartup() const {
    return format_sd_on_startup;
}

bool SystemConfiguration::IsFsBoardAvailable() const {
    return fs_board_available;
}

uint64_t SystemConfiguration::GetHalfOrbitPeriodMs() const {
    return half_orbit_period_ms;
}

uint32_t SystemConfiguration::GetHealthCheckPeriodMs() const {
    return health_check_period_ms;
}

bool SystemConfiguration::IsHilAvailable() const { return hil_available; }

uint16_t SystemConfiguration::GetIChargeHoldPollMs() const {
    return i_charge_hold_poll_ms;
}

uint16_t SystemConfiguration::GetIChargeHoldTimeMs() const {
    return i_charge_hold_time_ms;
}

bool SystemConfiguration::IsI2cAvailable() const { return i2c_available; }

uint64_t SystemConfiguration::GetInitialAntennaBurnIntervalMs() const {
    return initial_antenna_burn_interval_ms;
}

uint16_t SystemConfiguration::GetInitialIChargeIndex() const {
    return initial_i_charge_index;
}

bool SystemConfiguration::IsInstantDeploymentWaits() const {
    return instant_deployment_waits;
}

bool SystemConfiguration::IsLithiumTransmitOnlyWhenGroundCommanded() const {
    return lithium_transmit_only_when_ground_commanded;
}

bool SystemConfiguration::IsLogI2c() const { return log_i2c; }

bool SystemConfiguration::IsLogToSd() const { return log_to_sd; }

bool SystemConfiguration::IsLogToUart() const { return log_to_uart; }

float SystemConfiguration::GetMagnetorquerPowerFractionX() const {
    return magnetorquer_power_fraction_x;
}

float SystemConfiguration::GetMagnetorquerPowerFractionY() const {
    return magnetorquer_power_fraction_y;
}

float SystemConfiguration::GetMagnetorquerPowerFractionZ() const {
    return magnetorquer_power_fraction_z;
}

const double *SystemConfiguration::GetMaxMagnetorquerDipole() const {
    return max_magnetorquer_dipole;
}

uint32_t SystemConfiguration::GetNominalBeaconPeriodMs() const {
    return nominal_beacon_period_ms;
}

uint8_t SystemConfiguration::GetNominalLithiumPowerLevel() const {
    return nominal_lithium_power_level;
}

uint32_t SystemConfiguration::GetOrientationControlLoopPeriodMicros() const {
    return orientation_control_loop_period_micros;
}

double SystemConfiguration::GetOrientationControlPowerLevel() const {
    return orientation_control_power_level;
}

bool SystemConfiguration::IsRebootOnTopLevelException() const {
    return reboot_on_top_level_exception;
}

bool SystemConfiguration::IsRunMagnetorquersAtConstantPower() const {
    return run_magnetorquers_at_constant_power;
}

bool SystemConfiguration::IsSdCardAvailable() const {
    return sd_card_available;
}

uint32_t SystemConfiguration::GetSolarPowerRecoveryTimeMs() const {
    return solar_power_recovery_time_ms;
}

bool SystemConfiguration::IsTcomBoardAvailable() const {
    return tcom_board_available;
}

uint64_t SystemConfiguration::GetTimeSourceDeployMs() const {
    return time_source_deploy_ms;
}

uint32_t SystemConfiguration::GetTimeUpdatePeriodMs() const {
    return time_update_period_ms;
}

bool SystemConfiguration::IsUplinkFecEnabled() const {
    return uplink_fec_enabled;
}

bool SystemConfiguration::IsUtilitiesBoardAvailable() const {
    return utilities_board_available;
}

bool SystemConfiguration::IsVerboseLogging() const { return verbose_logging; }

bool SystemConfiguration::IsXNegAvailable() const { return x_neg_available; }

bool SystemConfiguration::IsXPosAvailable() const { return x_pos_available; }

bool SystemConfiguration::IsYNegAvailable() const { return y_neg_available; }

bool SystemConfiguration::IsYPosAvailable() const { return y_pos_available; }

bool SystemConfiguration::IsZNegAvailable() const { return z_neg_available; }

bool SystemConfiguration::IsZPosAvailable() const { return z_pos_available; }

uint32_t SystemConfiguration::GetChecksum() const { return checksum; }
