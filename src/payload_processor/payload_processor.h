#ifndef SRC_PAYLOAD_PROCESSOR_PAYLOAD_PROCESSOR_H_
#define SRC_PAYLOAD_PROCESSOR_PAYLOAD_PROCESSOR_H_

#include <src/util/data_types.h>
#include <src/util/message_codes.h>

class Uplink;

class PayloadProcessor {
   public:
    PayloadProcessor();
    bool ParseAndExecuteUplinks(byte* payload);
    static byte GetUplinkCodeLength();
    static byte GetEndTerminator();

   private:
    bool ParseNextUplinkAndExecute(byte& index, byte* payload);
    static Uplink* CreateUplink(uint16_t command_code, byte* payload);
    static constexpr byte kUplinkCodeLength = 2;
    static constexpr uint8_t kEndTerminator = 0;
};

#endif  //  SRC_PAYLOAD_PROCESSOR_PAYLOAD_PROCESSOR_H_
