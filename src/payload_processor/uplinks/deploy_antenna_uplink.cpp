#include <src/payload_processor/uplinks/deploy_antenna_uplink.h>
#include <src/util/nanopb_utils.h>
#include <src/telecomms/antenna.h>
#include <xdc/runtime/Log.h>

DeployAntennaUplink::DeployAntennaUplink(byte* payload)
    : Uplink(DeployAntennaUplinkPayload_size),
      deploy_antenna(NanopbDecode(DeployAntennaUplinkPayload)(payload)) {}

bool DeployAntennaUplink::ExecuteUplink() {
    if(deploy_antenna.deploy_antenna == kDeployUplinkCode) {
        Antenna::GetAntenna()->DeployAntenna();
        return Antenna::GetAntenna()->IsDoorsOpen();
    }
    Log_error0("Valid deploy antenna command code was not received.");
    return false;
}
