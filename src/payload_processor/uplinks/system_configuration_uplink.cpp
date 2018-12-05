#include <src/database/flash_memory/flash_storables/system_configuration.h>
#include <src/payload_processor/uplinks/system_configuration_uplink.h>
#include <src/util/nanopb_utils.h>
#include <src/util/reset_management.h>

SystemConfigurationUplink::SystemConfigurationUplink(byte* payload)
    : Uplink(SystemConfigurationUplinkPayload_size),
      decoded_payload(NanopbDecode(SystemConfigurationUplinkPayload)(payload)) {
}

bool SystemConfigurationUplink::ExecuteUplink() {
    SystemConfiguration new_config(decoded_payload);
    if (new_config.GetChecksum() != new_config.ComputeChecksum()) {
        return false;
    }
    new_config.StoreInFlash();
    ResetManagement::ResetSystemWithMessage(kForceResetCommandExecuted);
    // Shouldn't reach this line
    return false;
}
