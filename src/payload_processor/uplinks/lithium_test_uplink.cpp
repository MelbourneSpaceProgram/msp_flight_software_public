#include <external/nanopb/pb_decode.h>
#include <src/payload_processor/uplinks/lithium_test_uplink.h>
#include <src/telecomms/lithium.h>
#include <src/telecomms/lithium_commands/transmit_command.h>
#include <xdc/runtime/Log.h>

#include "../../telecomms/msp_payloads/pad_zeroes_payload.h"

LithiumTestUplink::LithiumTestUplink()
    : Uplink(kLithiumTestUplinkArgumentLength) {}

bool LithiumTestUplink::ExecuteUplink() {
    Lithium* lithium = Lithium::GetInstance();
    TestOnesPayload ones;
    bool success = true;
    for (uint8_t i = 0; i < kNumTestTransmits; i++) {
        if (!lithium->Transmit(&ones)) {
            success = false;
        }
    }
    return success;
}
