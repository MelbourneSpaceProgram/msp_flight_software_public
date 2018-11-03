#include <src/board/MSP432E.h>
#include <src/config/satellite.h>
#include <src/telecomms/lithium.h>
#include <src/telecomms/lithium_commands/transmit_command.h>
#include <src/telecomms/msp_payloads/beacon_payload.h>
#include <src/telecomms/runnable_beacon.h>
#include <src/util/task_utils.h>
#include <ti/drivers/GPIO.h>
#include <xdc/runtime/Log.h>

I2c *RunnableBeacon::beacon_bus_d;

uint32_t RunnableBeacon::beacon_period_ms = kNominalBeaconPeriodMs;

RunnableBeacon::RunnableBeacon() {}

fnptr RunnableBeacon::GetRunnablePointer() { return &Beacon; }

void RunnableBeacon::InitBusD(const I2c *bus_d) {
    beacon_bus_d = const_cast<I2c *>(bus_d);
}

void RunnableBeacon::Beacon() {
    Lithium *lithium = Lithium::GetInstance();

    while (1) {
        TaskUtils::SleepMilli(GetBeaconPeriodMs());

        BeaconPayload beacon;

        // TODO(dingbenjamin): Implement remaining beacon fields

        // Avoid building the packet if transmit is disabled
        if (lithium->IsTransmitEnabled()) {
            BeaconPayload beacon;
            if (lithium->Transmit(&beacon)) {
                Log_info0("Beacon transmission succeeded");
            } else {
                Log_error0("Beacon transmission failed");
            }

        } else {
            Log_info0("Beacon is disabled, did not transmit");
        }
    }
}

uint32_t RunnableBeacon::GetBeaconPeriodMs() { return beacon_period_ms; }
