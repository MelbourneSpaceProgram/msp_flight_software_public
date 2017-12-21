#ifndef SRC_MESSAGES_SERIALISED_MESSAGE_H_
#define SRC_MESSAGES_SERIALISED_MESSAGE_H_

#include <stdint.h>
#include "src/util/data_types.h"

class SerialisedMessage {
   public:
    static const uint16_t kMaxSerialisedSize = 300;
    byte buffer[kMaxSerialisedSize];
    uint16_t size;
    explicit SerialisedMessage(uint16_t size);
    SerialisedMessage(uint16_t size, byte* message);
};

#endif  // SRC_MESSAGES_SERIALISED_MESSAGE_H_
