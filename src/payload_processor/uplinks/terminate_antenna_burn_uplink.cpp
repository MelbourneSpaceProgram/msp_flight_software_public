#include <external/nanopb/pb_decode.h>
#include <src/database/flash_memory/flash_storables/antenna_burner_info.h>
#include <src/payload_processor/uplinks/terminate_antenna_burn_uplink.h>
#include <src/telecomms/runnable_antenna_burner.h>
#include <src/util/nanopb_utils.h>
#include <xdc/runtime/Log.h>

TerminateAntennaBurnUplink::TerminateAntennaBurnUplink(byte* payload)
    : Uplink(TerminateAntennaBurnPayload_size),
      decoded_command(NanopbDecode(TerminateAntennaBurnPayload)(payload)) {}

bool TerminateAntennaBurnUplink::ExecuteUplink() {
    if (kTerminateAntennaBurnDisableKey == decoded_command.terminate_burn ||
            kTerminateAntennaBurnEnableKey == decoded_command.terminate_burn) {
        AntennaBurnerInfo* antenna_burner_info =
            RunnableAntennaBurner::GetAntennaBurnerInfo();

        if (kTerminateAntennaBurnDisableKey == decoded_command.terminate_burn) {
            antenna_burner_info->SetAntennaBurnSetting(false);
        } else {
            antenna_burner_info->SetAntennaBurnSetting(true);
        }

        antenna_burner_info->StoreInFlash();
        return true;
    } else {
        Log_error0("Terminate antenna burn command has failed.");
        return false;
    }
}
