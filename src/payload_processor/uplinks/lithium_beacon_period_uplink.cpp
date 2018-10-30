#include <src/payload_processor/uplinks/lithium_beacon_period_uplink.h>
#include <src/telecomms/runnable_beacon.h>
#include <src/util/nanopb_utils.h>

LithiumBeaconPeriodUplink::LithiumBeaconPeriodUplink(byte* payload)
    : Uplink(LithiumBeaconPeriodUplinkPayload_size),
      beacon_period(NanopbDecode(LithiumBeaconPeriodUplinkPayload)(payload)) {}

bool LithiumBeaconPeriodUplink::ExecuteUplink() {
    RunnableBeacon::beacon_period_ms = beacon_period.beacon_period_ms;
    return true;
}
