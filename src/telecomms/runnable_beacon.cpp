#include <assert.h>
#include <src/config/satellite.h>
#include <src/telecomms/antenna.h>
#include <src/telecomms/lithium.h>
#include <src/telecomms/lithium_commands/transmit_command.h>
#include <src/telecomms/msp_payloads/beacon_payload.h>
#include <src/telecomms/msp_payloads/limp_mode_beacon_payload.h>
#include <src/telecomms/runnable_beacon.h>
#include <src/util/msp_exception.h>
#include <src/util/system_watchdog.h>
#include <src/util/tirtos_utils.h>
#include <xdc/runtime/Log.h>

uint32_t RunnableBeacon::beacon_period_ms = kNominalBeaconPeriodMs;
bool RunnableBeacon::limp_mode_beacon = false;
bool RunnableBeacon::initialised = false;

RunnableBeacon::RunnableBeacon(bool limp_mode) {
    assert(!initialised);
    limp_mode_beacon = limp_mode;
    initialised = true;
}

fnptr RunnableBeacon::GetRunnablePointer() { return &Beacon; }

void RunnableBeacon::Beacon() {
    Lithium* lithium = Lithium::GetInstance();

    while (1) {
        try {
            // TODO(dingbenjamin): Implement remaining beacon fields

            TirtosUtils::SleepMilli(beacon_period_ms -
                                    kSolarPowerRecoveryTimeMs);
            SystemWatchdog::ResetTimer();

            // Avoid building the packet if transmit is disabled
            if (lithium->IsTransmitEnabled()) {
                if (Antenna::GetAntenna()->IsBurning()) {
                    Log_info0("Aborting beacon while antenna is burning");
                    continue;
                }

                TransmitPayload* beacon = NULL;
                try {
                    if (!limp_mode_beacon) {
                        beacon = new BeaconPayload;
                    } else {
                        beacon = new LimpModeBeaconPayload;
                    }

                    if (lithium->Transmit(beacon)) {
                        Log_info0("Beacon transmission succeeded");
                    } else {
                        Log_error0("Beacon transmission failed");
                    }
                } catch (etl::exception& e) {
                    if (beacon != NULL) delete beacon;
                    throw;
                }
                if (beacon != NULL) delete beacon;
            } else {
                Log_info0("Beacon is disabled, did not transmit");
            }
        } catch (MspException& e) {
            MspException::LogTopLevelException(e, kRunnableBeaconCatch);
        }
    }
}

uint32_t RunnableBeacon::GetBeaconPeriodMs() { return beacon_period_ms; }
