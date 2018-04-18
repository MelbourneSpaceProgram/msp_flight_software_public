#ifndef SRC_PAYLOAD_PROCESSOR_PAYLOAD_PROCESSOR_H_
#define SRC_PAYLOAD_PROCESSOR_PAYLOAD_PROCESSOR_H_

#include <src/util/data_types.h>

class PayloadProcessor {
   public:
    bool ParseAndExecuteCommands(byte* payload);

   private:
    bool ParseNextCommandAndExecute(byte& index, byte* payload);
    enum Commands { kNoCommand = 0x01, kEchoCommand, kLithiumEnableCommand };
    static const uint8_t kCommandCodeLength = 2;
    static const byte kEndTerminator = 0x00;
};

#endif  //  SRC_PAYLOAD_PROCESSOR_PAYLOAD_PROCESSOR_H_
