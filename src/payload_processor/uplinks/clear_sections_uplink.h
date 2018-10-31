#ifndef SRC_PAYLOAD_PROCESSOR_COMMANDS_CLEAR_SECTION_BUFFER_COMMAND_H_
#define SRC_PAYLOAD_PROCESSOR_COMMANDS_CLEAR_SECTION_BUFFER_COMMAND_H_

#include <src/messages/ClearSectionsUplinkPayload.pb.h>
#include <src/payload_processor/uplinks/uplink.h>
#include <src/payload_processor/payload_processor.h>

class ClearSectionsUplink : public Uplink {
   public:
    ClearSectionsUplink(byte* payload,
                              PayloadProcessor* payload_processor);
    bool ExecuteUplink();

   private:
    PayloadProcessor* payload_processor;
    ClearSectionsUplinkPayload clear_buffer;
};

#endif  //  SRC_PAYLOAD_PROCESSOR_COMMANDS_CLEAR_SECTION_BUFFER_COMMAND_H_
