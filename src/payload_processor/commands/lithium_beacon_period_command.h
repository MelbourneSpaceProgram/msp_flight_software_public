#ifndef SRC_PAYLOAD_PROCESSOR_COMMANDS_LITHIUM_BEACON_PERIOD_COMMAND_H_
#define SRC_PAYLOAD_PROCESSOR_COMMANDS_LITHIUM_BEACON_PERIOD_COMMAND_H_

#include <src/messages/LithiumBeaconPeriodCommandPayload.pb.h>
#include <src/payload_processor/commands/command.h>

class LithiumBeaconPeriodCommand : public Command {
   public:
    explicit LithiumBeaconPeriodCommand(byte* payload);
    bool ExecuteCommand();

   private:
    LithiumBeaconPeriodCommandPayload beacon_period;
};
#endif  // SRC_PAYLOAD_PROCESSOR_COMMANDS_LITHIUM_BEACON_PERIOD_COMMAND_H_
