#include <src/payload_processor/uplinks/lithium_get_configuration_uplink.h>
#include <src/telecomms/lithium.h>
#include <src/telecomms/lithium_commands/get_configuration_command.h>
#include <src/telecomms/msp_payloads/lithium_get_configuration_uplink_payload.h>
#include <xdc/runtime/Log.h>

LithiumGetConfigurationUplink::LithiumGetConfigurationUplink() : Uplink(0) {}

bool LithiumGetConfigurationUplink::ExecuteUplink() {
    GetConfigurationCommand get_config_command;
    if (!Lithium::GetInstance()->DoCommand(&get_config_command)) {
        Log_error0("Unable to retrieve config from Lithium");
        return false;
    }

    LithiumGetConfigurationUplinkPayload payload(get_config_command.GetParsedResponse());
    return Lithium::GetInstance()->Transmit(&payload);
}
