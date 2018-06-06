#include <src/payload_processor/commands/uplink_speed_test_command.h>
#include <xdc/runtime/Log.h>
#include <string.h>

UplinkSpeedTestCommand::UplinkSpeedTestCommand(byte* payload, uint8_t command_argument_start_index)
    : Command(kUplinkSpeedTestCommandArgumentLength) {

    // Use this if the memcpy fails
     //for(int i = 0; i < kUplinkSpeedTestCommandArgumentLength; i++){
     //    command_arguments.insert_at(i, (payload + command_argument_start_index)[i]);
     //}

    memcpy(&command_arguments, payload + command_argument_start_index, kUplinkSpeedTestCommandArgumentLength);
}

bool UplinkSpeedTestCommand::ExecuteCommand() {

    uint32_t packet_counter = (command_arguments[0] << 24)
            + (command_arguments[1] << 16)
            + (command_arguments[2] << 8)
            + (command_arguments[3] << 0);

    Log_info1("Uplink packet counter = %d", packet_counter);

    return true;
}
