#ifndef SRC_DATABASE_FLASH_MEMORY_FLASH_STORABLES_SYSTEM_CONFIGURATION_H_
#define SRC_DATABASE_FLASH_MEMORY_FLASH_STORABLES_SYSTEM_CONFIGURATION_H_

#include <src/database/flash_memory/flash_storables/flash_storable.h>
#include <src/database/flash_memory/flash_storables/flash_storable_data_types.h>
#include <src/messages/SystemConfigurationUplinkPayload.pb.h>

class SystemConfiguration : public FlashStorable<SystemConfiguration> {
	friend class FlashStorable<SystemConfiguration>;
	friend class SystemConfigurationUplink;

   public:
    static SystemConfiguration *GetInstance();
    bool IsValid() const;

    bool IsAntennaAvailable() const;
    uint64_t GetAntennaBurnCheckIntervalMs() const;
    float GetAntennaBurnIntervalMultiplier() const;
    uint64_t GetBackupAntennaBurnIntervalMs() const;
    uint32_t GetBeaconWaitMs() const;
    bool IsBmsAvailable() const;
    bool IsCdhBoardAvailable() const;
    bool IsCheckHmacDefault() const;
    bool IsCheckSequenceDefault() const;
    bool IsDeployAntenna() const;
    bool IsDitlMode() const;
    bool IsDownlinkFecEnabled() const;
    bool IsEepromAvailable() const;
    uint32_t GetEjectionWaitMs() const;
    bool IsEnterDeepSleepOnStartup() const;
    bool IsEnterLimpModeOnStartup() const;
    bool IsEpsBoardAvailable() const;
    bool IsFormatSdOnStartup() const;
    bool IsFsBoardAvailable() const;
    uint64_t GetHalfOrbitPeriodMs() const;
    uint32_t GetHealthCheckPeriodMs() const;
    bool IsHilAvailable() const;
    uint16_t GetIChargeHoldPollMs() const;
    uint16_t GetIChargeHoldTimeMs() const;
    bool IsI2cAvailable() const;
    uint64_t GetInitialAntennaBurnIntervalMs() const;
    uint16_t GetInitialIChargeIndex() const;
    bool IsInstantDeploymentWaits() const;
    bool IsLithiumTransmitOnlyWhenGroundCommanded() const;
    bool IsLogI2c() const;
    bool IsLogToSd() const;
    bool IsLogToUart() const;
    float GetMagnetorquerPowerFractionX() const;
    float GetMagnetorquerPowerFractionY() const;
    float GetMagnetorquerPowerFractionZ() const;
    const double *GetMaxMagnetorquerDipole() const;
    uint32_t GetNominalBeaconPeriodMs() const;
    uint8_t GetNominalLithiumPowerLevel() const;
    uint32_t GetOrientationControlLoopPeriodMicros() const;
    double GetOrientationControlPowerLevel() const;
    bool IsRebootOnTopLevelException() const;
    bool IsRunMagnetorquersAtConstantPower() const;
    bool IsSdCardAvailable() const;
    uint32_t GetSolarPowerRecoveryTimeMs() const;
    bool IsTcomBoardAvailable() const;
    uint64_t GetTimeSourceDeployMs() const;
    uint32_t GetTimeUpdatePeriodMs() const;
    bool IsUplinkFecEnabled() const;
    bool IsUtilitiesBoardAvailable() const;
    bool IsVerboseLogging() const;
    bool IsXNegAvailable() const;
    bool IsXPosAvailable() const;
    bool IsYNegAvailable() const;
    bool IsYPosAvailable() const;
    bool IsZNegAvailable() const;
    bool IsZPosAvailable() const;

    uint32_t GetChecksum() const;

   private:
	SystemConfiguration();
    SystemConfiguration(bool, bool use_defaults = false);
	explicit SystemConfiguration(SystemConfigurationUplinkPayload& payload);
    void ConvertToFlashStorableStruct(
        SystemConfiguration *system_configuration_struct);
    void UpdateFromFlashStorableStruct(
        SystemConfiguration *system_configuration_struct);
    uint32_t ComputeChecksum();

    bool eps_board_available = true;
    bool cdh_board_available = true;
    bool fs_board_available = true;
    bool tcom_board_available = true;
    bool utilities_board_available = true;
    bool x_pos_available = true;
    bool x_neg_available = true;
    bool y_pos_available = true;
    bool y_neg_available = true;
    bool z_pos_available = true;
    bool z_neg_available = true;
    bool i2c_available = true;
    bool sd_card_available = true;
    bool bms_available = true;
    bool antenna_available = false;
    bool eeprom_available = false;
    bool enter_deep_sleep_on_startup = false;
    bool enter_limp_mode_on_startup = false;
    bool deploy_antenna = false;  // DO NOT TURN ON UNTIL AFTER LAUNCH
    bool instant_deployment_waits = true;
    bool format_sd_on_startup = false;
    uint32_t beacon_wait_ms = 5 * 60 * 1000;
    uint32_t ejection_wait_ms = 30 * 60 * 1000;
    uint64_t time_source_deploy_ms = 0;
    bool lithium_transmit_only_when_ground_commanded = false;
    uint8_t nominal_lithium_power_level = 140;  // Scaled between 0-255
    bool uplink_fec_enabled = true;
    bool downlink_fec_enabled = false;
    bool check_hmac_default = true;
    bool check_sequence_default = false;
    bool log_i2c = false;
    bool log_to_uart = true;
    bool log_to_sd = true;
    bool hil_available = false;
    bool ditl_mode = false;
    bool verbose_logging = false;
    bool reboot_on_top_level_exception = true;
    uint32_t nominal_beacon_period_ms = 10000;
    uint32_t orientation_control_loop_period_micros = 1000000;
    uint32_t health_check_period_ms = 1100;
    uint64_t antenna_burn_check_interval_ms = 60 * 1000;
    uint32_t time_update_period_ms = 1000;
    uint16_t i_charge_hold_time_ms = 5000;
    uint16_t i_charge_hold_poll_ms = 200;
    uint32_t solar_power_recovery_time_ms = 300;
    uint16_t initial_i_charge_index = 1;
    uint64_t half_orbit_period_ms = 46.5 * 60 * 1000;
    uint64_t initial_antenna_burn_interval_ms = half_orbit_period_ms;
    uint64_t backup_antenna_burn_interval_ms = 3 * half_orbit_period_ms;
    float antenna_burn_interval_multiplier = 2.5;
    bool run_magnetorquers_at_constant_power = true;
    float magnetorquer_power_fraction_x = 0.2;
    float magnetorquer_power_fraction_y = 0.2;
    float magnetorquer_power_fraction_z = 0.2;
    double max_magnetorquer_dipole[3] = {0.2, 0.2, 0.2};
    double orientation_control_power_level = 0.2;

    uint32_t checksum;
};

#endif  // SRC_DATABASE_FLASH_MEMORY_FLASH_STORABLES_SYSTEM_CONFIGURATION_H_
