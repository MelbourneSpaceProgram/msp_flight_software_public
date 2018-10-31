#ifndef SRC_PAYLOAD_PROCESSOR_COMMANDS_POST_SECTION_BUFFER_COMMAND_H_
#define SRC_PAYLOAD_PROCESSOR_COMMANDS_POST_SECTION_BUFFER_COMMAND_H_

#include <src/messages/ExecuteSectionsUplinkPayload.pb.h>
#include <src/payload_processor/uplinks/uplink.h>
#include <src/payload_processor/payload_processor.h>

class ExecuteSectionsUplink : public Uplink {
   public:
    ExecuteSectionsUplink(byte* payload,
                              PayloadProcessor* payload_processor);
    bool ExecuteUplink();

   private:
    PayloadProcessor* payload_processor;
    ExecuteSectionsUplinkPayload post_buffer;
};

#endif  //  SRC_PAYLOAD_PROCESSOR_COMMANDS_POST_SECTION_BUFFER_COMMAND_H_
