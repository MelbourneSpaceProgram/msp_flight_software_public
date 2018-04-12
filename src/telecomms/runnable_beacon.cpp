#include <src/sensors/i2c_measurable_manager.h>
#include <src/telecomms/lithium.h>
#include <src/telecomms/runnable_beacon.h>
#include <src/util/task_utils.h>
#include <ti/sysbios/knl/Task.h>
#include <xdc/runtime/Log.h>

RunnableBeacon::RunnableBeacon() {}

fnptr RunnableBeacon::GetRunnablePointer() { return &Beacon; }

void RunnableBeacon::Beacon() {
    Lithium *lithium = Lithium::GetInstance();
    I2cMeasurableManager *manager = I2cMeasurableManager::GetInstance();
    uint8_t tx_count = 0;
    while (1) {
        BeaconPayload beacon;
        // Cast the double reading to a 8 bit unsigned int
        beacon.SetCdhRegTemp1(static_cast<byte>(
            manager->ReadI2cMeasurable<double>(kCdhTemp1, 0)));

        TransmitCommand transmit_command(&beacon, tx_count++, 0, 0);
        if (!lithium->DoCommand(&transmit_command)) {
            Log_error0("Beacon transmission failed");
        }
        TaskUtils::SleepMilli(10000);
    }
}
