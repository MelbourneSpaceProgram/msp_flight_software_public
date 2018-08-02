#ifndef SRC_PAYLOAD_PROCESSOR_PAYLOAD_PROCESSOR_H_
#define SRC_PAYLOAD_PROCESSOR_PAYLOAD_PROCESSOR_H_

#include <src/util/data_types.h>

class PayloadProcessor {
   public:
    PayloadProcessor();
    bool ParseAndExecuteCommands(byte* payload);
    static byte GetCommandCodeLength();
    static byte GetEndTerminator();

   private:
    bool ParseNextCommandAndExecute(byte& index, byte* payload);
    enum Commands {
        kNoCommand = 0,
        kEchoCommand = 1,
        kLithiumEnableCommand = 2,
        kTleUpdateCommand = 3,
        kForceResetCommand = 4,
        kCommandsLength = 5
    };
    static const byte kCommandCodeLength = 2;
    static const uint8_t kEndTerminator = 0;
};

#endif  //  SRC_PAYLOAD_PROCESSOR_PAYLOAD_PROCESSOR_H_
