#include <src/messages/LithiumMd5Payload.pb.h>
#include <src/payload_processor/uplinks/lithium_write_flash_uplink.h>
#include <src/telecomms/lithium.h>
#include <src/telecomms/lithium_commands/no_op_command.h>
#include <src/telecomms/lithium_commands/write_flash_command.h>
#include <src/util/msp_exception.h>
#include <src/util/nanopb_utils.h>
#include <src/util/satellite_power.h>
#include <src/util/tirtos_utils.h>
#include <ti/sysbios/gates/GateMutexPri.h>

LithiumWriteFlashUplink::LithiumWriteFlashUplink(byte* payload)
    : Uplink(kLithiumWriteFlashUplinkArgumentLength),
      md5(NanopbDecode(LithiumMd5Payload)(payload)) {}

bool LithiumWriteFlashUplink::ExecuteUplink() {
    bool success = Lithium::GetInstance()->DoWriteFlash(md5);

    IArg key = SatellitePower::Lock();
    try {
        SatellitePower::CutPowerToTelecoms();
        TirtosUtils::SleepMilli(1000);
        SatellitePower::RestorePowerToTelecoms();
    } catch (MspException& e) {
		SatellitePower::Unlock(key);
		throw;
    }
    SatellitePower::Unlock(key);

    success = success && Lithium::GetInstance()->DoNoOp();
    return success;
}
