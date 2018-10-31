#include <src/messages/serialised_message_builder.h>
#include <src/telecomms/lithium_md5.h>
#include <src/util/nanopb_utils.h>

LithiumMd5::LithiumMd5(etl::array<byte, kNumMd5Bytes>* md5_bytes)
    : md5_bytes(md5_bytes), heap_allocated(false) {}

LithiumMd5::LithiumMd5(LithiumMd5Payload payload)
    : md5_bytes(NULL), heap_allocated(true) {
    md5_bytes = new etl::array<byte, kNumMd5Bytes>;

    for (uint8_t i = 0; i < kNumMd5Bytes; ++i) {
        md5_bytes->insert_at(i, payload.checksum[i]);
    }
}

LithiumMd5::~LithiumMd5() {
    if (heap_allocated) {
        delete md5_bytes;
    }
}

SerialisedMessage LithiumMd5::SerialiseTo(byte* serial_buffer) const {
    SerialisedMessageBuilder builder(serial_buffer, kNumMd5Bytes);
    for (uint8_t i = 0; i < kNumMd5Bytes; i++) {
        builder.AddData<byte>((*md5_bytes)[i]);
    }
    return builder.Build();
}

uint16_t LithiumMd5::GetSerialisedSize() const { return kNumMd5Bytes; }
