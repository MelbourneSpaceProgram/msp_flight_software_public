#ifndef SRC_MESSAGES_SERIALISED_MESSAGE_H_
#define SRC_MESSAGES_SERIALISED_MESSAGE_H_

#include <stdint.h>
#include "src/util/data_types.h"

class SerialisedMessage {
   public:
    byte* buffer;
    uint16_t size;
    explicit SerialisedMessage(uint16_t size);
    SerialisedMessage(uint16_t size, byte* message);
    ~SerialisedMessage();
};

#endif /* SRC_MESSAGES_SERIALISED_MESSAGE_H_ */
