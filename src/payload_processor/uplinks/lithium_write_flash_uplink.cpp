#include <src/messages/LithiumMd5Payload.pb.h>
#include <src/payload_processor/uplinks/lithium_write_flash_uplink.h>
#include <src/telecomms/lithium.h>
#include <src/telecomms/lithium_commands/no_op_command.h>
#include <src/telecomms/lithium_commands/write_flash_command.h>
#include <src/util/msp_exception.h>
#include <src/util/nanopb_utils.h>
#include <src/util/satellite_power.h>
#include <src/util/task_utils.h>
#include <ti/sysbios/gates/GateMutexPri.h>

LithiumWriteFlashUplink::LithiumWriteFlashUplink(byte* payload)
    : Uplink(kLithiumWriteFlashUplinkArgumentLength),
      md5(NanopbDecode(LithiumMd5Payload)(payload)) {}

bool LithiumWriteFlashUplink::ExecuteUplink() {
    WriteFlashCommand command(&md5);
    bool success = Lithium::GetInstance()->DoCommand(&command);

    IArg key = SatellitePower::Lock();
    try {
        SatellitePower::CutPowerToTelecoms();
        TaskUtils::SleepMilli(1000);
        SatellitePower::RestorePowerToTelecoms();
    } catch (MspException& e) {
		SatellitePower::Unlock(key);
		throw;
    }
    SatellitePower::Unlock(key);

    NoOpCommand no_op;
    success = success && Lithium::GetInstance()->DoCommand(&no_op);
    return success;
}
