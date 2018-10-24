#ifndef SRC_PAYLOAD_PROCESSOR_PAYLOAD_PROCESSOR_H_
#define SRC_PAYLOAD_PROCESSOR_PAYLOAD_PROCESSOR_H_

#include <src/util/data_types.h>
#include <src/util/message_codes.h>

class Command;

class PayloadProcessor {
   public:
    PayloadProcessor();
    bool ParseAndExecuteCommands(byte* payload);
    static byte GetCommandCodeLength();
    static byte GetEndTerminator();

   private:
    bool ParseNextCommandAndExecute(byte& index, byte* payload);
    static Command* CreateCommand(uint16_t command_code, byte* payload);
    static constexpr byte kCommandCodeLength = 2;
    static constexpr uint8_t kEndTerminator = 0;
};

#endif  //  SRC_PAYLOAD_PROCESSOR_PAYLOAD_PROCESSOR_H_
