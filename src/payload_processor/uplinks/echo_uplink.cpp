#include <src/payload_processor/uplinks/echo_uplink.h>

EchoUplink::EchoUplink(byte* payload)
  : Uplink(EchoPayload::kEchoMessageLength),
    response(payload) { }

bool EchoUplink::ExecuteUplink() {
    return Lithium::GetInstance()->Transmit(&response);
}
