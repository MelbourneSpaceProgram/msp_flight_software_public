#include <src/sensors/i2c_measurable_manager.h>
#include <src/telecomms/lithium.h>
#include <src/telecomms/lithium_commands/transmit_command.h>
#include <src/telecomms/msp_payloads/beacon_payload.h>
#include <src/telecomms/runnable_beacon.h>
#include <src/util/task_utils.h>
#include <xdc/runtime/Log.h>

RunnableBeacon::RunnableBeacon() {}

fnptr RunnableBeacon::GetRunnablePointer() { return &Beacon; }

void RunnableBeacon::Beacon() {
    Lithium *lithium = Lithium::GetInstance();
    I2cMeasurableManager *manager = I2cMeasurableManager::GetInstance();
    uint8_t tx_count = 0;

    while (1) {
        BeaconPayload beacon;

        // TODO(dingbenjamin): Implement remaining beacon fields
        // Cast the double reading to a 8 bit unsigned int
        beacon.SetCdhRegTemp1(static_cast<byte>(
            manager->ReadI2cMeasurable<double>(kCdhSysTemp, 0)));

        // Avoid building the packet if transmit is disabled
        if (lithium->IsTransmitEnabled()) {
            TransmitCommand transmit_command(&beacon, tx_count++, 0, 0);
            if (lithium->DoCommand(&transmit_command)) {
                Log_info0("Beacon transmission succeeded");
            } else {
                Log_error0("Beacon transmission failed");
            }
        } else {
            Log_info0("Beacon is disabled, did not transmit");
        }
        TaskUtils::SleepMilli(2000);
    }
}
