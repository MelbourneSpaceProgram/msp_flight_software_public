#ifndef SERIALISED_MESSAGE_H_
#define SERIALISED_MESSAGE_H_

#include <stdint.h>
#include "src/util/data_types.h"

class SerialisedMessage {
public:
    byte* buffer;
    uint16_t size;
    SerialisedMessage(uint16_t size);
    SerialisedMessage(uint16_t size, byte* message);
    virtual ~SerialisedMessage();
};

#endif /* SERIALISED_MESSAGE_H_ */
