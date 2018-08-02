#ifndef SRC_PAYLOAD_PROCESSOR_COMMANDS_LITHIUM_BEACON_PERIOD_COMMAND_H_
#define SRC_PAYLOAD_PROCESSOR_COMMANDS_LITHIUM_BEACON_PERIOD_COMMAND_H_

#include <src/messages/LithiumBeaconPeriod.pb.h>
#include <src/payload_processor/commands/command.h>

class LithiumBeaconPeriodCommand : public Command {
   public:
    LithiumBeaconPeriodCommand(byte* payload);
    bool ExecuteCommand();

   private:
    LithiumBeaconPeriod beacon_period;
    static const uint8_t kLithiumBeaconPeriodCommandArgumentLength =
        LithiumBeaconPeriod_size;
};
#endif  // SRC_PAYLOAD_PROCESSOR_COMMANDS_LITHIUM_BEACON_PERIOD_COMMAND_H_
