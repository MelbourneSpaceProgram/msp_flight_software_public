#include <src/adcs/runnable_orientation_control.h>
#include <src/config/satellite.h>
#include <src/telecomms/lithium.h>
#include <src/telecomms/lithium_commands/transmit_command.h>
#include <src/telecomms/msp_payloads/beacon_payload.h>
#include <src/telecomms/runnable_beacon.h>
#include <src/util/satellite_power.h>
#include <src/util/task_utils.h>
#include <ti/sysbios/knl/Semaphore.h>
#include <xdc/runtime/Log.h>

uint32_t RunnableBeacon::beacon_period_ms = kNominalBeaconPeriodMs;

RunnableBeacon::RunnableBeacon() {}

fnptr RunnableBeacon::GetRunnablePointer() { return &Beacon; }

void RunnableBeacon::Beacon() {
    Lithium *lithium = Lithium::GetInstance();

    while (1) {
        // TODO(dingbenjamin): Implement remaining beacon fields

        TaskUtils::SleepMilli(beacon_period_ms);
        // Avoid building the packet if transmit is disabled
        if (lithium->IsTransmitEnabled()) {
            BeaconPayload beacon;

            SatellitePower::CutPowerToFlightSystems();
            if (lithium->Transmit(&beacon)) {
                Log_info0("Beacon transmission succeeded");
            } else {
                Log_error0("Beacon transmission failed");
            }
            SatellitePower::RestorePowerToFlightSystems();
            Semaphore_post(RunnableOrientationControl::beacon_over_semaphore);
        } else {
            Log_info0("Beacon is disabled, did not transmit");
        }
    }
}

uint32_t RunnableBeacon::GetBeaconPeriodMs() { return beacon_period_ms; }
