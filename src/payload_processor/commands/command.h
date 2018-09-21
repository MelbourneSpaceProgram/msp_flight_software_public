#ifndef SRC_PAYLOAD_PROCESSOR_COMMANDS_COMMAND_H_
#define SRC_PAYLOAD_PROCESSOR_COMMANDS_COMMAND_H_

#include <src/util/data_types.h>

class Command {
   public:
    explicit Command(uint8_t command_arg_length);
    virtual ~Command() {};
    virtual bool ExecuteCommand() = 0;
    uint8_t GetCommandArgumentLength() const;

   protected:
    uint8_t command_arg_length;
};

#endif  //  SRC_PAYLOAD_PROCESSOR_COMMANDS_COMMAND_H_
