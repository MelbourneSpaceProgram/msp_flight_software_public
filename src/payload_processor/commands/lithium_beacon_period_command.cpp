#include <external/nanopb/pb_decode.h>
#include <src/config/satellite.h>
#include <src/payload_processor/commands/lithium_beacon_period_command.h>
#include <src/telecomms/lithium.h>
#include <src/telecomms/runnable_beacon.h>
#include <src/util/nanopb_utils.h>
#include <xdc/runtime/Log.h>

LithiumBeaconPeriodCommand::LithiumBeaconPeriodCommand(byte* payload)
    : Command(kLithiumBeaconPeriodCommandArgumentLength),
      beacon_period(NanopbDecode(LithiumBeaconPeriod)(payload)) {}

bool LithiumBeaconPeriodCommand::ExecuteCommand() {
    RunnableBeacon::beacon_period_ms = beacon_period.beacon_period_ms;
    return true;
}
