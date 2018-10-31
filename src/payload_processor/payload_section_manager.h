#ifndef SRC_PAYLOAD_PROCESSOR_PAYLOAD_SECTION_MANAGER_H_
#define SRC_PAYLOAD_PROCESSOR_PAYLOAD_SECTION_MANAGER_H_

#include <external/etl/array.h>
#include <external/etl/vector.h>
#include <src/telecomms/lithium.h>
#include <src/util/data_types.h>
#include <ti/sysbios/knl/Mailbox.h>
#include <utility>

class PayloadSectionManager {
   public:
    PayloadSectionManager();
    bool AddSectionToBuffer(uint16_t command_code, uint16_t start_index,
                            uint16_t end_index, byte* section);
    void CopyToBuffer(uint16_t from_index, uint16_t to_index, byte* section,
                      uint16_t section_offset);
    bool PostPayloadToUplinkMailbox();
    void ClearBuffer();
    byte* GetBuffer();
    etl::vector<std::pair<uint16_t, uint16_t>, Lithium::kMaxReceivedUplinkSize>
    GetBufferStatus();
    uint16_t GetCurrentCommandCode();

   private:
    uint16_t current_command_code;
    Mailbox_Handle uplink_mailbox_handle;
    byte section_buffer[Lithium::kMaxReceivedUplinkSize];
    etl::vector<std::pair<uint16_t, uint16_t>, Lithium::kMaxReceivedUplinkSize>
        empty_locations;
};

#endif  //  SRC_PAYLOAD_PROCESSOR_PAYLOAD_SECTION_MANAGER_H_
