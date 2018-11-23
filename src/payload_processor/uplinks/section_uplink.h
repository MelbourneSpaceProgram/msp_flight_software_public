#ifndef SRC_PAYLOAD_PROCESSOR_COMMANDS_UPLINK_SECTION_COMMAND_H_
#define SRC_PAYLOAD_PROCESSOR_COMMANDS_UPLINK_SECTION_COMMAND_H_

#include <src/messages/SectionUplinkPayload.pb.h>
#include <src/payload_processor/payload_processor.h>
#include <src/payload_processor/uplinks/uplink.h>

class SectionUplink : public Uplink {
   public:
    SectionUplink(byte* payload, PayloadProcessor* payload_processor);
    bool ExecuteUplink();

   private:
    SectionUplinkPayload uplink_section_command;
    PayloadProcessor* payload_processor;
};

#endif  //  SRC_PAYLOAD_PROCESSOR_COMMANDS_UPLINK_SECTION_COMMAND_H_
