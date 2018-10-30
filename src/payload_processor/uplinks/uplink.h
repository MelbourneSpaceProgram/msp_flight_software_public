#ifndef SRC_PAYLOAD_PROCESSOR_COMMANDS_UPLINK_H_
#define SRC_PAYLOAD_PROCESSOR_COMMANDS_UPLINK_H_

#include <src/util/data_types.h>

class Uplink {
   public:
    explicit Uplink(uint8_t command_arg_length);
    virtual ~Uplink() {};
    virtual bool ExecuteUplink() = 0;
    uint8_t GetUplinkArgumentLength() const;

   protected:
    uint8_t command_arg_length;
};

#endif  //  SRC_PAYLOAD_PROCESSOR_COMMANDS_UPLINK_H_
