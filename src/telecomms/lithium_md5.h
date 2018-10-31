#ifndef SRC_TELECOMMS_LITHIUM_MD5_H_
#define SRC_TELECOMMS_LITHIUM_MD5_H_

#include <external/etl/array.h>
#include <src/messages/message.h>
#include <src/util/data_types.h>
#include <src/messages/LithiumMd5Payload.pb.h>

class LithiumMd5 : public Message {
   public:
    static constexpr uint8_t kNumMd5Bytes = 16;

    explicit LithiumMd5(etl::array<byte, kNumMd5Bytes>* md5_bytes);
	explicit LithiumMd5(LithiumMd5Payload payload);	
	~LithiumMd5();
    SerialisedMessage SerialiseTo(byte* serial_buffer) const;
    uint16_t GetSerialisedSize() const;

   private:
    etl::array<byte, kNumMd5Bytes>* md5_bytes;
	bool heap_allocated;
};

#endif  // SRC_TELECOMMS_LITHIUM_MD5_H_
