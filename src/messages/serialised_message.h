#ifndef SRC_MESSAGES_SERIALISED_MESSAGE_H_
#define SRC_MESSAGES_SERIALISED_MESSAGE_H_

#include <stdint.h>
#include "src/util/data_types.h"

class SerialisedMessage {
   public:
    uint16_t size;
    byte* buffer;
    explicit SerialisedMessage(uint16_t size);
    SerialisedMessage(uint16_t size, byte* message);
    ~SerialisedMessage();

   private:
    SerialisedMessage(const SerialisedMessage&);
    SerialisedMessage& operator=(const SerialisedMessage&);
};

#endif  // SRC_MESSAGES_SERIALISED_MESSAGE_H_
