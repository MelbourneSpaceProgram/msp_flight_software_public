#include <src/config/satellite.h>
#include <src/telecomms/lithium.h>
#include <src/telecomms/lithium_commands/transmit_command.h>
#include <src/telecomms/msp_payloads/beacon_payload.h>
#include <src/telecomms/runnable_beacon.h>
#include <src/util/msp_exception.h>
#include <src/util/satellite_power.h>
#include <src/util/task_utils.h>
#include <xdc/runtime/Log.h>

uint32_t RunnableBeacon::beacon_period_ms = kNominalBeaconPeriodMs;

RunnableBeacon::RunnableBeacon() {}

fnptr RunnableBeacon::GetRunnablePointer() { return &Beacon; }

void RunnableBeacon::Beacon() {
    Lithium* lithium = Lithium::GetInstance();

    while (1) {
        // TODO(dingbenjamin): Implement remaining beacon fields

        // Avoid building the packet if transmit is disabled
        if (lithium->IsTransmitEnabled()) {
            BeaconPayload beacon;
            try {
                SatellitePower::CutPowerToFlightSystems();
            } catch (etl::exception& e) {
                MspException::LogException(e);
            }
            if (lithium->Transmit(&beacon)) {
                Log_info0("Beacon transmission succeeded");
            } else {
                Log_error0("Beacon transmission failed");
            }
            try {
                SatellitePower::RestorePowerToFlightSystems();
            } catch (etl::exception& e) {
                MspException::LogException(e);
            }
        } else {
            Log_info0("Beacon is disabled, did not transmit");
        }
        TaskUtils::SleepMilli(beacon_period_ms);
    }
}

uint32_t RunnableBeacon::GetBeaconPeriodMs() { return beacon_period_ms; }
