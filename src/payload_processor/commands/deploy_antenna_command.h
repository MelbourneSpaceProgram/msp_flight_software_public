#ifndef SRC_PAYLOAD_PROCESSOR_COMMANDS_ANTENNA_DEPLOY_COMMAND_H_
#define SRC_PAYLOAD_PROCESSOR_COMMANDS_ANTENNA_DEPLOY_COMMAND_H_

#include <src/messages/DeployAntennaCommandPayload.pb.h>
#include <src/payload_processor/commands/command.h>

class DeployAntennaCommmand : public Command {
   public:
    explicit DeployAntennaCommmand(byte* payload);
    bool ExecuteCommand();

   private:
    DeployAntennaCommandPayload deploy_antenna;
    static const uint32_t kDeployCommandCode = 0x01234567;
};

#endif  // SRC_PAYLOAD_PROCESSOR_COMMANDS_ANTENNA_DEPLOY_COMMAND_H_
