#ifndef SRC_PAYLOAD_PROCESSOR_PAYLOAD_PROCESSOR_H_
#define SRC_PAYLOAD_PROCESSOR_PAYLOAD_PROCESSOR_H_

#include <src/payload_processor/payload_section_manager.h>
#include <src/payload_processor/runnable_payload_processor.h>
#include <src/util/data_types.h>
#include <src/util/message_codes.h>

class Uplink;

class PayloadProcessor {
    friend class SectionUplink;
    friend class ClearSectionsUplink;
    friend class ExecuteSectionsUplink;
    friend class QuerySectionsUplink;

   public:
    PayloadProcessor();
    bool ParseAndExecuteUplinks(byte* payload);
    static byte GetUplinkCodeLength();
    static byte GetEndTerminator();

    static constexpr uint8_t kUplinkCodeLength = 2;
    static constexpr uint8_t kUplinkArgumentsMaxLength = RunnablePayloadProcessor::kMspUplinkMaxLength - kUplinkCodeLength;

   private:
    bool ParseNextUplinkAndExecute(uint16_t& index, byte* payload);
    PayloadSectionManager* GetPayloadSectionManager();
    PayloadSectionManager payload_section_manager;
    Uplink* CreateUplink(uint16_t command_code, byte* payload);

    static constexpr uint8_t kEndTerminator = 0;
};

#endif  //  SRC_PAYLOAD_PROCESSOR_PAYLOAD_PROCESSOR_H_
