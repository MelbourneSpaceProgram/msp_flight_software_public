#include <src/board/board.h>
#include <src/config/satellite.h>
#include <src/messages/TemperatureReading.pb.h>
#include <src/messages/serialised_message.h>
#include <src/sensors/measurable_manager.h>
#include <src/telecomms/lithium.h>
#include <src/telecomms/lithium_commands/fast_pa_command.h>
#include <src/telecomms/lithium_commands/get_configuration_command.h>
#include <src/telecomms/lithium_commands/lithium_command.h>
#include <src/telecomms/lithium_commands/lithium_command_codes.h>
#include <src/telecomms/lithium_commands/no_op_command.h>
#include <src/telecomms/lithium_commands/reset_system_command.h>
#include <src/telecomms/lithium_commands/set_configuration_command.h>
#include <src/telecomms/lithium_commands/telemetry_query_command.h>
#include <src/telecomms/lithium_commands/transmit_command.h>
#include <src/telecomms/lithium_commands/write_flash_command.h>
#include <src/telecomms/lithium_configuration.h>
#include <src/telecomms/lithium_md5.h>
#include <src/telecomms/lithium_telemetry.h>
#include <src/telecomms/lithium_utils.h>
#include <src/util/data_types.h>
#include <src/util/msp_exception.h>
#include <src/util/satellite_power.h>
#include <src/util/task_utils.h>
#include <ti/drivers/GPIO.h>
#include <xdc/runtime/Log.h>

Lithium* Lithium::instance = NULL;
uint8_t Lithium::tx_count = 0;
uint8_t Lithium::rx_count = 0;
uint8_t Lithium::command_success_count = 0;
IArg Lithium::power_key = 0;
bool Lithium::currently_transmitting = false;

Lithium::Lithium()
    : uart(TELECOMS),
      lithium_transmit_enabled(!kLithiumTransmitOnlyWhenGroundCommanded),
      lock(kLithiumOnCondition),
      state(kLithiumTempNominal),
      consecutive_serial_failures(0),
      mutex_params(),
      lithium_mutex(NULL) {
    // Ensure Lithium is not in reset
    GPIO_write(nCOMMS_RST, 1);

    uart.SetBaudRate(Uart::kBaud9600)
        ->SetReadMode(UART_MODE_BLOCKING)
        ->SetWriteMode(UART_MODE_BLOCKING)
        ->SetWriteTimeout(TaskUtils::MilliToCycles(kUartWriteTimeoutMilli))
        ->SetReadTimeout(TaskUtils::MilliToCycles(kUartReadTimeoutMilli))
        ->Open();

    Mailbox_Params_init(&command_response_mailbox_params);
    command_response_mailbox_handle =
        Mailbox_create(kMaxReceivedLithiumResponseSize, 1,
                       &command_response_mailbox_params, NULL);
    if (command_response_mailbox_handle == NULL) {
        throw MspException("Unable to create Lithium command response mailbox",
                           kLithiumCommandResponseMailboxFail, __FILE__,
                           __LINE__);
    }

    Mailbox_Params_init(&header_mailbox_params);
    header_mailbox_handle =
        Mailbox_create(kLithiumHeaderSize, 1, &header_mailbox_params, NULL);
    if (header_mailbox_handle == NULL) {
        throw MspException("Unable to create Lithium header mailbox",
                           kLithiumHeaderMailboxFail, __FILE__, __LINE__);
    }

    Mailbox_Params_init(&uplink_mailbox_params);
    uplink_mailbox_handle =
        Mailbox_create(kMaxReceivedUplinkSize, kMaxNumberOfPayloads,
                       &uplink_mailbox_params, NULL);
    if (uplink_mailbox_handle == NULL) {
        throw MspException("Unable to create Lithium uplink mailbox",
                           kLithiumUplinkMailboxFail, __FILE__, __LINE__);
    }

    GateMutexPri_Params_init(&mutex_params);
    lithium_mutex = GateMutexPri_create(&mutex_params, NULL);
    if (lithium_mutex == NULL) {
        throw MspException("Failed to create Lithium mutex", kLithiumMutexFail,
                           __FILE__, __LINE__);
    }
}

void Lithium::PreTransmit() {
    // Turn off flight systems before every transmission to avoid booting with
    // flight systems on in the case of a brownout.
    power_key = SatellitePower::Lock();
    try {
        SatellitePower::CutPowerToFlightSystems();
    } catch (MspException& e) {
        MspException::LogException(e, kLithiumPreTransmitCatch);
    }
}

void Lithium::PostTransmit() {
    try {
        SatellitePower::RestorePowerToFlightSystems();
    } catch (MspException& e) {
        MspException::LogException(e, kLithiumPostTransmit1Catch);
    }

    // Re-write the configuration to the BMSs in the event that
    // one of the batteries has died during transmission.
    // If one battery dies while the other is discharging, its BMS will reboot
    // and use the unsupportable charging current from its hardware settings
    // until re-configured.
    try {
        TaskUtils::SleepMilli(kSolarPowerRecoveryTimeMs);
        if (!SatellitePower::ConfigureBms(SatellitePower::kBmsBusD)) {
            Log_error0("Failure to configure BMS on bus D");
        }
        if (!SatellitePower::ConfigureBms(SatellitePower::kBmsBusC)) {
            Log_error0("Failure to configure BMS on bus C");
        }
    } catch (MspException& e) {
        MspException::LogException(e, kLithiumPostTransmit2Catch);
    }
    SatellitePower::Unlock(power_key);
}

bool Lithium::Transmit(TransmitPayload* transmit_payload) {
    UpdateState();
    if (!lithium_transmit_enabled) {
        Log_info0("Attempted to transmit, but transmit disabled");
        return false;
    }

    currently_transmitting = true;
    PreTransmit();

    TransmitCommand transmit_command(transmit_payload);
    bool transmit_successful =
        Lithium::GetInstance()->DoCommand(&transmit_command);

    PostTransmit();
    currently_transmitting = false;

    if (transmit_successful) {
        tx_count = tx_count == 255 ? 0 : tx_count + 1;
        return true;
    }
    return false;
}

bool Lithium::DoCommand(LithiumCommand* command) {
    uint16_t serialised_size = command->GetSerialisedSize();
    if (serialised_size >= kMaxOutgoingCommandSize) {
        return false;
    }
    byte command_buffer[kMaxOutgoingCommandSize];
    SerialisedMessage serial_command = command->SerialiseTo(command_buffer);

    IArg key = GateMutexPri_enter(lithium_mutex);

    try {
        if (!uart.PerformWriteTransaction(serial_command.GetBuffer(),
                                          serial_command.GetSize())) {
            FailSerial(key);
            return false;
        }

        byte ack_buffer[kLithiumHeaderSize] = {0};
        if (!Mailbox_pend(header_mailbox_handle, &ack_buffer,
                          TaskUtils::MilliToCycles(kWaitForAckMilli))) {
            // Timed out waiting for a response
            FailSerial(key);
            return false;
        }
        // TODO(dingbenjamin): Figure out why this is needed
        TaskUtils::SleepMilli(kInterCommandTimeMilli);

        if ((!LithiumUtils::IsValidHeader(ack_buffer)) ||
            (LithiumUtils::GetCommandCode(ack_buffer) !=
             command->GetCommandCode())) {
            FailSerial(key);
            return false;
        }

        if (command->GetReplyPayloadSize() == 0) {
            // No response payload is expected
            if (LithiumUtils::IsAck(ack_buffer)) {
                SuccessSerial(key);
                return true;
            } else if (LithiumUtils::IsNack(ack_buffer)) {
                FailSerial(key);
                return false;
            } else {
                FailSerial(key);
                return false;
            }
        } else {
            // Expecting more than just an ack from the Lithium
            if (LithiumUtils::GetPayloadSize(ack_buffer) ==
                command->GetReplyPayloadSize()) {
                if (Mailbox_pend(command_response_mailbox_handle,
                                 command->GetReplyBuffer(),
                                 kWaitForReplyPayloadMilli)) {
                    SuccessSerial(key);
                    return true;
                } else {
                    FailSerial(key);
                    return false;
                }
            } else {
                FailSerial(key);
                return false;
            }
        }
    } catch (MspException& e) {
        MspException::LogException(e, kLithiumDoCommandCatch);
        GateMutexPri_leave(lithium_mutex, key);
        return false;
    }
}

Lithium* Lithium::GetInstance() {
    if (instance == NULL) {
        instance = new Lithium();
    }
    return instance;
}

uint8_t Lithium::GetTxCounter() { return Lithium::tx_count; }

uint8_t Lithium::GetRxCounter() { return Lithium::rx_count; }

uint8_t Lithium::GetCommandSuccessCounter() {
    return Lithium::command_success_count;
}

bool Lithium::DoTelemetryQuery(LithiumTelemetry& returned_telemetry) {
    TelemetryQueryCommand query;

    if (!DoCommand(&query))
        // Sometimes the Lithium returns a telemetry packet filled with
        // zeroes, if this is the case we should query again
        if (!TelemetryQueryCommand::CheckValidTelemetry(
                query.GetParsedResponse())) {
            // Try again if we get an invalid telemetry
            if (!DoCommand(&query)) return false;
            if (!TelemetryQueryCommand::CheckValidTelemetry(
                    query.GetParsedResponse()))
                return false;
        }
    // We have received a valid telemetry
    returned_telemetry = query.GetParsedResponse();
    return true;
}

bool Lithium::DoGetConfiguration(LithiumConfiguration& returned_configuration) {
    GetConfigurationCommand get_config_command;
    if (!DoCommand(&get_config_command)) {
        Log_error0("Unable to retrieve config from Lithium");
        return false;
    }

    returned_configuration = get_config_command.GetParsedResponse();
    return true;
}

bool Lithium::DoSetConfiguration(LithiumConfiguration config) {
    SetConfigurationCommand set_config_command(config);
    return DoCommand(&set_config_command);
}

bool Lithium::DoWriteFlash(LithiumMd5 md5) {
    WriteFlashCommand flash_command(&md5);
    return DoCommand(&flash_command);
}

bool Lithium::DoFastPa(uint8_t pa_level) {
    FastPaCommand fast_pa_command(pa_level);
    return DoCommand(&fast_pa_command);
}

bool Lithium::DoNoOp() {
    NoOpCommand no_op;
    return DoCommand(&no_op);
}

void Lithium::UnlockState(LithiumShutoffCondition condition) {
    lock = lock & ~static_cast<uint8_t>(condition);

    if (lock == kLithiumOnCondition) {
        lithium_transmit_enabled = true;
    }
}
void Lithium::LockState(LithiumShutoffCondition condition) {
    lock = lock | static_cast<uint8_t>(condition);

    if (lock != kLithiumOnCondition) {
        lithium_transmit_enabled = false;
    }
}

bool Lithium::IsStateLocked(LithiumShutoffCondition condition) {
    return (lock & static_cast<uint8_t>(condition)) != kLithiumOnCondition;
}

void Lithium::ForceUnlock() { lock = kLithiumOnCondition; }

void Lithium::UpdateState() {
    try {
        // TODO(dingbenjamin): Use the Lithium internal temperature sensor
        float telecomms_1_temp =
            MeasurableManager::GetInstance()
                ->ReadNanopbMeasurable<TemperatureReading>(kComT1, 10000)
                .temp;
        float telecomms_2_temp =
            MeasurableManager::GetInstance()
                ->ReadNanopbMeasurable<TemperatureReading>(kComT2, 10000)
                .temp;

        // Disregard invalid sensor readings
        if (telecomms_1_temp == kInvalidDouble)
            telecomms_1_temp = kLithiumTempOperationalNominal;
        if (telecomms_2_temp == kInvalidDouble)
            telecomms_2_temp = kLithiumTempOperationalNominal;

        switch (state) {
            case kLithiumTempCriticalLow:
                if ((telecomms_1_temp > kLithiumTempOperationalMin) &&
                    (telecomms_2_temp > kLithiumTempOperationalMin)) {
                    Log_info0(
                        "Telecomms temperature nominal: turning on "
                        "Lithium");
                    Lithium::GetInstance()->UnlockState(
                        Lithium::kCriticalTempCondition);
                }
                break;
            case kLithiumTempNominal:
                if ((telecomms_1_temp >
                     kLithiumTempOperationalMax + kHysteresis) ||
                    (telecomms_2_temp >
                     kLithiumTempOperationalMax + kHysteresis)) {
                    Log_info0(
                        "Telecomms temperature critical high: Shutting off "
                        "Lithium");
                    Lithium::GetInstance()->LockState(
                        Lithium::kCriticalTempCondition);

                } else if ((telecomms_1_temp <
                            kLithiumTempOperationalMin - kHysteresis) ||
                           (telecomms_2_temp <
                            kLithiumTempOperationalMin - kHysteresis)) {
                    Log_info0(
                        "Telecomms temperature critical low: Shutting off "
                        "Lithium");
                    Lithium::GetInstance()->LockState(
                        Lithium::kCriticalTempCondition);
                }
                break;
            case kLithiumTempCriticalHigh:
                if ((telecomms_1_temp < kLithiumTempOperationalMax) &&
                    (telecomms_2_temp < kLithiumTempOperationalMax)) {
                    Log_info0(
                        "Telecomms temperature nominal: turning on "
                        "Lithium");
                    Lithium::GetInstance()->UnlockState(
                        Lithium::kCriticalTempCondition);
                }
                break;
        }
    } catch (MspException& e) {
        MspException::LogException(e, kLithiumUpdateStateCatch);
        Log_info0("Failed to update lithium state, ensuring it is turned on");
        Lithium::GetInstance()->UnlockState(Lithium::kCriticalTempCondition);
    }
}

Uart* Lithium::GetUart() { return &uart; }

Mailbox_Handle Lithium::GetUplinkMailbox() const {
    return uplink_mailbox_handle;
}

Mailbox_Handle Lithium::GetHeaderMailbox() const {
    return header_mailbox_handle;
}

Mailbox_Handle Lithium::GetCommandResponseMailbox() const {
    return command_response_mailbox_handle;
}

void Lithium::SetTransmitEnabled(bool lithium_enabled) {
    lithium_transmit_enabled = lithium_enabled;
}

bool Lithium::IsTransmitEnabled() { return lithium_transmit_enabled; }

bool Lithium::IsTransmitting() { return currently_transmitting; }

void Lithium::FailSerial(IArg key) {
    if (consecutive_serial_failures >= kFailsBeforeReset) {
        Log_warning1(
            "%d consecutive failures communicating with Lithium, resetting",
            consecutive_serial_failures);
        consecutive_serial_failures = 0;

        SatellitePower::CutPowerToTelecoms();
        TaskUtils::SleepMilli(kResetWaitMs);
        SatellitePower::RestorePowerToTelecoms();
    } else {
        consecutive_serial_failures++;
    }
    GateMutexPri_leave(lithium_mutex, key);
}

void Lithium::SuccessSerial(IArg key) {
    consecutive_serial_failures = 0;
    GateMutexPri_leave(lithium_mutex, key);
}
