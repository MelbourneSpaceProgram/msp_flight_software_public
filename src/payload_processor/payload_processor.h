#ifndef SRC_PAYLOAD_PROCESSOR_PAYLOAD_PROCESSOR_H_
#define SRC_PAYLOAD_PROCESSOR_PAYLOAD_PROCESSOR_H_

#include <src/util/data_types.h>

class PayloadProcessor {
   public:
    bool ParseAndExecuteCommands(byte* payload);

   private:
    bool ParseNextCommandAndExecute(byte& index, byte* payload);
    enum Commands { kNoCommand, kEchoCommand, kCommandsLength };
    static const byte kCommandLength = 1;
    static const uint8_t kEndTerminator = 0;
};

#endif  //  SRC_PAYLOAD_PROCESSOR_PAYLOAD_PROCESSOR_H_
