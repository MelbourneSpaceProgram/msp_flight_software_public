#include <external/nanopb/pb_decode.h>
#include <src/payload_processor/commands/lithium_test_command.h>
#include <src/telecomms/lithium.h>
#include <src/telecomms/lithium_commands/transmit_command.h>
#include <src/telecomms/msp_payloads/test_ones_payload.h>
#include <xdc/runtime/Log.h>

LithiumTestCommand::LithiumTestCommand()
    : Command(kLithiumTestCommandArgumentLength) {}

bool LithiumTestCommand::ExecuteCommand() {
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
