#include <src/payload_processor/commands/lithium_beacon_period_command.h>
#include <src/telecomms/runnable_beacon.h>
#include <src/util/nanopb_utils.h>

LithiumBeaconPeriodCommand::LithiumBeaconPeriodCommand(byte* payload)
    : Command(LithiumBeaconPeriodCommandPayload_size),
      beacon_period(NanopbDecode(LithiumBeaconPeriodCommandPayload)(payload)) {}

bool LithiumBeaconPeriodCommand::ExecuteCommand() {
    RunnableBeacon::beacon_period_ms = beacon_period.beacon_period_ms;
    return true;
}
