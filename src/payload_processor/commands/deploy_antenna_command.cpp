#include <src/payload_processor/commands/deploy_antenna_command.h>
#include <src/util/nanopb_utils.h>
#include <src/telecomms/antenna.h>
#include <xdc/runtime/Log.h>

DeployAntennaCommmand::DeployAntennaCommmand(byte* payload)
    : Command(DeployAntennaCommandPayload_size),
      deploy_antenna(NanopbDecode(DeployAntennaCommandPayload)(payload)) {}

bool DeployAntennaCommmand::ExecuteCommand() {
    if(deploy_antenna.deploy_antenna == kDeployCommandCode) {
        Antenna::GetAntenna()->DeployAntenna();
        return Antenna::GetAntenna()->IsDoorsOpen();
    }
    Log_error0("Valid deploy antenna command code was not received.");
    return false;
}
