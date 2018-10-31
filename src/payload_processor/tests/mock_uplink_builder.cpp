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

byte* MockUplinkBuilder::GetMockUplinkBuffer() const {
    return uplink_buffer;
}

byte* MockUplinkBuilder::Build() {
    AddData<uint8_t>(PayloadProcessor::GetEndTerminator());
    AddData<uint8_t>(PayloadProcessor::GetEndTerminator());
    return uplink_buffer;
}
