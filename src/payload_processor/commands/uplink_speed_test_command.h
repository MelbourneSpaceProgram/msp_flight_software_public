#ifndef SRC_PAYLOAD_PROCESSOR_COMMANDS_UPLINK_SPEED_TEST_COMMAND_H_
#define SRC_PAYLOAD_PROCESSOR_COMMANDS_UPLINK_SPEED_TEST_COMMAND_H_

#include <src/payload_processor/commands/command.h>
#include <external/etl/array.h>

class UplinkSpeedTestCommand : public Command {
public:
    UplinkSpeedTestCommand(byte* payload, uint8_t command_argument_start_index);
    bool ExecuteCommand();

private:
    static const uint8_t kUplinkSpeedTestCommandArgumentLength = 70 * 3 + 4;
    etl::array<byte, kUplinkSpeedTestCommandArgumentLength> command_arguments;
};

#endif  //  SRC_PAYLOAD_PROCESSOR_COMMANDS_UPLINK_SPEED_TEST_COMMAND_H_
