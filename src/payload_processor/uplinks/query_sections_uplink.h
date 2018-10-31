#ifndef SRC_PAYLOAD_PROCESSOR_COMMANDS_QUERY_SECTION_BUFFER_COMMAND_H_
#define SRC_PAYLOAD_PROCESSOR_COMMANDS_QUERY_SECTION_BUFFER_COMMAND_H_

#include <src/messages/QuerySectionsUplinkPayload.pb.h>
#include <src/payload_processor/payload_processor.h>
#include <src/payload_processor/uplinks/uplink.h>

class QuerySectionsUplink : public Uplink {
   public:
    QuerySectionsUplink(byte* payload, PayloadProcessor* payload_processor);
    bool ExecuteUplink();

   private:
    QuerySectionsUplinkPayload queried;
    PayloadProcessor* payload_processor;
};

#endif  //  SRC_PAYLOAD_PROCESSOR_COMMANDS_QUERY_SECTION_BUFFER_COMMAND_H_
