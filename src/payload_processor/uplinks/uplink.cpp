#include <src/payload_processor/uplinks/uplink.h>

Uplink::Uplink(uint8_t command_arg_length)
    : command_arg_length(command_arg_length) {}

uint8_t Uplink::GetUplinkArgumentLength() const { return command_arg_length; }
