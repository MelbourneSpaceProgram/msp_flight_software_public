#ifndef SRC_MESSAGES_MOCK_UPLINK_BUILDER_H_
#define SRC_MESSAGES_MOCK_UPLINK_BUILDER_H_

#include <external/etl/exception.h>
#include <src/payload_processor/payload_processor.h>
#include <src/util/data_types.h>
#include <src/util/message_codes.h>
#include <src/util/nanopb_utils.h>
#include <string.h>

#define AddNanopbMacro(NanopbMessageType)                  \
    AddNanopb<NanopbMessageType, NanopbMessageType##_size, \
              NanopbMessageType##_fields>

class MockUplinkBuilder {
   public:
    MockUplinkBuilder(byte* serial_buffer, uint16_t buffer_size);
    uint16_t GetSerialisedLength() const;
    byte* GetMockUplinkBuffer() const;
    byte* Build();  // Lifetime of result is tied to this object
    MockUplinkBuilder& AddUplinkCode(Uplinks code);
    MockUplinkBuilder& AddUplinkSection(MockUplinkBuilder* builder,
                                        uint16_t start_index,
                                        uint16_t end_index);

    template <class T>
    MockUplinkBuilder& AddData(const T data) {
        if (sizeof(T) > buffer_size - serialised_length) {
            etl::exception e("Message builder buffer size overflow", __FILE__,
                             __LINE__);
            throw e;
        } else {
            memcpy(uplink_buffer + serialised_length, &data, sizeof(T));
            serialised_length += sizeof(T);
        }
        return *this;
    }

    template <typename NanopbMessageType, uint16_t NanopbMessageType_size,
              const pb_field_t* NanopbMessageType_fields>
    MockUplinkBuilder& AddNanopb(const NanopbMessageType data) {
        if (NanopbMessageType_size > buffer_size - serialised_length) {
            etl::exception e("Message builder buffer size overflow", __FILE__,
                             __LINE__);
            throw e;
        } else {
            NanopbUtils::Encode<NanopbMessageType, NanopbMessageType_size,
                                NanopbMessageType_fields>(
                uplink_buffer + serialised_length, data);
            serialised_length += NanopbMessageType_size;
        }
        return *this;
    }

   private:
    MockUplinkBuilder(const MockUplinkBuilder&);
    MockUplinkBuilder& operator=(const MockUplinkBuilder&);
    uint16_t buffer_size;
    byte* uplink_buffer;
    uint16_t serialised_length;
};

#endif  // SRC_MESSAGES_MOCK_UPLINK_BUILDER_H_
