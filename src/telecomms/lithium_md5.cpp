#include <src/messages/serialised_message_builder.h>
#include <src/telecomms/lithium_md5.h>

LithiumMd5::LithiumMd5(etl::array<byte, kNumMd5Bytes>* md5_bytes)
    : md5_bytes(md5_bytes) {}

SerialisedMessage LithiumMd5::SerialiseTo(byte* serial_buffer) const {
    SerialisedMessageBuilder builder(serial_buffer, kNumMd5Bytes);
    for (uint8_t i = 0; i < kNumMd5Bytes; i++) {
        builder.AddData<byte>((*md5_bytes)[i]);
    }
    return builder.Build();
}

uint16_t LithiumMd5::GetSerialisedSize() const { return kNumMd5Bytes; }
