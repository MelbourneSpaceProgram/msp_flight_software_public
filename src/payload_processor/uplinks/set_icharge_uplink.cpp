#include <external/nanopb/pb_decode.h>
#include <src/payload_processor/uplinks/set_icharge_uplink.h>
#include <src/util/nanopb_utils.h>
#include <src/util/satellite_power.h>

SetIChargeUplink::SetIChargeUplink(byte* payload)
    : Uplink(SetIChargePayload_size),
      decoded_command(NanopbDecode(SetIChargePayload)(payload)) {}

bool SetIChargeUplink::ExecuteUplink() {
    SatellitePower::BmsId bms_id =
        static_cast<SatellitePower::BmsId>(decoded_command.bms_id);
    if (bms_id <= SatellitePower::kNumBms) {
        Bms* bms = SatellitePower::GetBms(bms_id);
        return bms->SetICharge(decoded_command.i_charge_index);
    }
    return false;
}
