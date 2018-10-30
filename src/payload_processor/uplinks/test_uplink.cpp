#include <src/payload_processor/uplinks/test_uplink.h>

TestUplink::TestUplink(byte* payload) : Uplink(kEchoUplinkArgumentLength) {
    command_arguments[0] = payload[0];
    command_arguments[1] = payload[1];
}

bool TestUplink::ExecuteUplink() {
    if (command_arguments[0] == kTestValue1 &&
        command_arguments[1] == kTestValue2) {
        return true;
    }
    return false;
}
