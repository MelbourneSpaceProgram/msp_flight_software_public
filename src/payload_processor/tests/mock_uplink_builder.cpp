#include <src/messages/SectionUplinkPayload.pb.h>
#include <src/payload_processor/tests/mock_uplink_builder.h>
#include <stdint.h>

MockUplinkBuilder::MockUplinkBuilder(byte* serial_buffer, uint16_t buffer_size)
    : buffer_size(buffer_size),
      uplink_buffer(serial_buffer),
      serialised_length(0) {}

uint16_t MockUplinkBuilder::GetSerialisedLength() const {
    return serialised_length;
}

MockUplinkBuilder& MockUplinkBuilder::AddUplinkCode(Uplinks code) {
    uint16_t uplink_16 = static_cast<uint16_t>(code);
    AddData<uint16_t>(uplink_16);
    return *this;
}

byte* MockUplinkBuilder::GetMockUplinkBuffer() const { return uplink_buffer; }

// Assumes only one command in the builder argument, which is complete and has a
// code in the first two bytes
MockUplinkBuilder& MockUplinkBuilder::AddUplinkSection(
    MockUplinkBuilder* builder, uint16_t start_index, uint16_t end_index) {
    // TODO(dingbenjamin): Limit checking
    SectionUplinkPayload section;
    section.command_code =
        *reinterpret_cast<uint16_t*>(builder->GetMockUplinkBuffer());
    section.start_index = start_index;
    section.end_index = end_index;
    uint16_t section_size = end_index - start_index + 1;
    section.section.size = section_size;

    for (uint16_t i = 0; i < section_size; ++i) {
        section.section.bytes[i] =
            builder->GetMockUplinkBuffer()[i + start_index];
    }
    AddNanopbMacro(SectionUplinkPayload)(section);

    return *this;
}

byte* MockUplinkBuilder::Build() {
    AddData<uint8_t>(PayloadProcessor::GetEndTerminator());
    AddData<uint8_t>(PayloadProcessor::GetEndTerminator());
    return uplink_buffer;
}
